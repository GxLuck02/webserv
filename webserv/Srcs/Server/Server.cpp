/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttreichl <ttreichl@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 18:26:59 by ttreichl          #+#    #+#             */
/*   Updated: 2025/09/21 16:29:58 by ttreichl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "RecieveRequest.hpp"
#include "Parser.hpp"

Server::Server(std::string configFille)
{
	std::cout << "Server default constructor called." << std::endl;
	ConfigParser input(configFille);
	this->_servs = input.getConfigs();
	if (this->_servs.empty())
		throw std::runtime_error("No server configurations found.");
	else
	{
		for (size_t i = 0; i < this->_servs.size(); i++)
		{
			std::cout << "Parsed ////" << this->_servs[i] << " //////\n";
		}
	}	
}

Server::~Server()
{
	for (size_t i = 0; i < this->_servs.size(); ++i)
	{
		if (this->_servs[i].getListenFd() != -1)
		{
			int fd = this->_servs[i].getListenFd();
			close(fd);
			std::cout << "Closing server socket: " << fd << std::endl;
		}
	}
	this->_servs.clear();
	for (size_t i = 0; i < this->_clients.size(); ++i)
	{
		if(this->_clients[i]->getFd() != -1)
		{
			close(this->_clients[i]->getFd());
			std::cout << "Closing client fd: " << this->_clients[i]->getFd() << std::endl;
		}
		delete this->_clients[i];
	}
	this->_clients.clear();
	this->_poll_fds.clear();
	std::cout << "Server destructor called." << std::endl;
}

Server::Server(const Server &other)
{
	(void)other;
	throw std::runtime_error("Server is non-copyable");
}

Server &Server::operator=(const Server &other)
{
	(void)other;
	throw std::runtime_error("Server is non-assignable");
	return *this;
}

/************************** Geters and Setters ***************************/

// Get the client object associated with a given file descriptor
Client *Server::getClient(int fd) const
{
	for (size_t i = 0; i < this->_clients.size(); ++i)
	{
		if (this->_clients[i]->getFd() == fd)
			return this->_clients[i];
	}
	throw std::runtime_error("Error: Client not found.");
}

// Get the index of the poll file descriptor for a given file descriptor
int Server::getIndexPollFd(int fd) const
{
	for (size_t i = 0; i < this->_poll_fds.size(); ++i)
	{
		if (this->_poll_fds[i].fd == fd)
			return i;
	}
	throw std::runtime_error("Error: Poll fd not found.");
}

/*************************** Gesture of Server ***************************/

// Initialize the server
// This function creates a socket, sets it to non-blocking mode, binds it to the specified IP and port, and starts listening for incoming connections.
struct PortIp {
    int port;
    in_addr_t ip;
    int fd;
};


void Server::initServer()
{
    std::vector<PortIp> existingSockets;

    for (size_t i = 0; i < this->_servs.size(); ++i)
    {
        // Cherche un fd existant pour ce port/ip
        int reuse_fd = -1;
        for (size_t j = 0; j < existingSockets.size(); ++j)
        {
            if (existingSockets[j].port == this->_servs[i].getPort() &&
                existingSockets[j].ip   == this->_servs[i].getIp())
            {
                reuse_fd = existingSockets[j].fd;
                break;
            }
        }

        if (reuse_fd != -1)
        {
            // On réutilise le socket déjà écoutant
            this->_servs[i].setListenFd(reuse_fd);
            std::cout << "Server socket already listening on port "
                      << this->_servs[i].getPort() << std::endl;
            continue;
        }

        // Sinon on crée un nouveau socket
        int fd = socket(AF_INET, SOCK_STREAM, 0);
        if (fd < 0)
            throw std::runtime_error("Error: cannot create socket");

        this->_servs[i].setListenFd(fd);

        int opt = 1;
        if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
            throw std::runtime_error("Error: setsockopt failed");

        setSocketNonBlocking(i);

        // bind
        sockaddr_in address;
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = this->_servs[i].getIp();
        address.sin_port = htons(this->_servs[i].getPort());
        if (bind(fd, (struct sockaddr *)&address, sizeof(address)) < 0)
            throw std::runtime_error("Error: bind failed");

        // listen
        if (listen(fd, SOMAXCONN) < 0)
            throw std::runtime_error("Error: listen failed");

        std::cout << "Server number: " << i << " initialized on port "
                  << this->_servs[i].getPort() << std::endl;

        // Enregistre ce fd pour les futurs serveurs partageant ce port/ip
        PortIp entry;
        entry.port = this->_servs[i].getPort();
        entry.ip = this->_servs[i].getIp();
        entry.fd = fd;
        existingSockets.push_back(entry);
    }
}

// Run the server
// This function initializes the server, sets up the poll file descriptors, and enters the main loop
void Server::run()
{
	std::cout << "Server is running..." << std::endl;
	this->addServerToPollFds();
	while (true)
	{
		this->checkTimeouts();
		int poll_count = poll(this->_poll_fds.data(), this->_poll_fds.size(), 900);
		if (poll_count < 0)
			throw std::runtime_error("Error: poll failed.");
		for (size_t i = 0; i < this->_poll_fds.size(); ++i)
		{
			if (this->_poll_fds[i].revents & POLLIN)
			{
				if (this->checkItsServerSocket(i) == true)
				{
					std::cout << "New connection on server socket: " << this->_poll_fds[i].fd << std::endl;
					this->acceptNewClient(i);
				}
				else
				{
					std::cout << "Handling read for client fd: " << this->_poll_fds[i].fd << std::endl;
					this->handleClientRead(this->_poll_fds[i].fd);
				}
			}
			if (this->_poll_fds[i].revents & POLLOUT)
			{
				std::cout << "Handling write for client fd: " << this->_poll_fds[i].fd << std::endl;
				this->handleClientWrite(this->_poll_fds[i].fd);
			}
			if (this->_poll_fds[i].revents & (POLLHUP | POLLERR | POLLNVAL))
			{
				std::cerr << "Error or hangup on client fd: " << this->_poll_fds[i].fd << std::endl;
				this->removeClient(this->_poll_fds[i].fd);
			}
			this->_poll_fds[i].revents = 0; // Reset revents for next poll
		}
	}
	this->endRunLoop();
}

// End the run loop
// This function clears the poll_fds and clients, effectively ending the run loop.
void Server::endRunLoop()
{
	for (size_t i = 0; i < this->_clients.size(); ++i)
	{
		if (!this->_clients[i]->isClosed())
		{
			close(this->_clients[i]->getFd());
			std::cout << "Closing client fd: " << this->_clients[i]->getFd() << std::endl;
		}
	}	
	this->_clients.clear();
	this->_poll_fds.clear();
	std::cout << "Server run loop ended." << std::endl;
	for (size_t i = 0; i < this->_servs.size(); ++i)
	{
		if (this->_servs[i].getListenFd() != -1)
		{
			int fd = this->_servs[i].getListenFd();
			close(fd);
			std::cout << "Closing server socket: " << fd << std::endl;
		}
	}
	this->_servs.clear();
}
// Set the server socket to non-blocking mode
// This function uses fcntl to set the socket to non-blocking mode, allowing the server to handle multiple clients without blocking on read/write operations.
void Server::setSocketNonBlocking(int index)
{
	if (fcntl(this->_servs[index].getListenFd(), F_SETFL, O_NONBLOCK) < 0)
	{
		close(this->_servs[index].getListenFd());
		throw std::runtime_error("Error: fcntl F_SETFL failed.");
	}
	std::cout << "Server socket set to non-blocking mode." << std::endl;
}

// Bind the server socket to the specified IP and port
// This function creates a sockaddr_in structure with the server's IP and port, and binds the socket to it.
void Server::bindSocket(int index)
{
	sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = this->_servs[index].getIp();
	address.sin_port = htons(this->_servs[index].getPort());

	// Only bind if no other server is already bound to this IP:port
	if (!checkIfServeSocketAlreadyExists(this->_servs, index))
	{
		if (bind(this->_servs[index].getListenFd(), (struct sockaddr *)&address, sizeof(address)) < 0)
		{
			close(this->_servs[index].getListenFd());
			throw std::runtime_error("Error: error in bind of serversocket.");
		}
	}
	// Otherwise, do not reset listenFd, just use the already bound fd
	std::cout << "Server number: " << index << " initialized on port " << this->_servs[index].getPort() << std::endl;
	std::cout << "Server IP: " << inet_ntoa(address.sin_addr) << std::endl;
}

// Listen for incoming connections on the server socket
// This function sets the server socket to listen for incoming connections with a maximum queue size of SOM
void Server::listenSocket(int index)
{
	// Check if another server with the same IP:port has already called listen()
	int listen_fd = this->_servs[index].getListenFd();
	for (size_t i = 0; i < this->_servs.size(); ++i)
	{
		if (i != static_cast<size_t>(index) &&
			this->_servs[i].getPort() == this->_servs[index].getPort() &&
			this->_servs[i].getIp() == this->_servs[index].getIp() &&
			this->_servs[i].getListenFd() == listen_fd)
		{
			std::cout << "Server socket already listening on port " << this->_servs[index].getPort() << std::endl;
			return;
		}
	}

	// If no one is listening yet -> call listen()
	if (listen(listen_fd, SOMAXCONN) < 0)
	{
		close(listen_fd);
		throw std::runtime_error("Error: error in listen of serversocket.");
	}
}

// Add the server to the poll file descriptors
void Server::addServerToPollFds()
{
	std::set<int> added;
	for (size_t i = 0; i < this->_servs.size(); ++i)
	{
		int fd = this->_servs[i].getListenFd();
        if (added.find(fd) != added.end())
            continue;
		struct pollfd serv_poll_fd;
		serv_poll_fd.fd = this->_servs[i].getListenFd();
		serv_poll_fd.events = POLLIN;
		serv_poll_fd.revents = 0;
		this->_poll_fds.push_back(serv_poll_fd);
		added.insert(fd);
		std::cout << "Server added to poll_fds with fd: " << serv_poll_fd.fd << std::endl;
	}
}

// Check if the poll file descriptor corresponds to a server socket
bool Server::checkItsServerSocket(int index) const
{
	for (size_t i = 0; i < this->_servs.size(); ++i)
	{
		if (this->_poll_fds[index].fd == this->_servs[i].getListenFd())
		{
			return true;
		}
	}
	return false;
}

/**************************** Gesture of Clients *****************************************/

// Handle reading data from the client
// This function reads data from the client socket, updates the client's last activity time, and checks
void Server::handleClientRead(int fd)
{
	Client *current_client = this->getClient(fd);
	char	buffer[1024];
	
	ssize_t byte_read = 0;
	byte_read = recv(fd, buffer, sizeof(buffer), 0);
	if (byte_read == 0)
	{
		std::cerr << "Client fd: " << fd << " closed connection." << std::endl;
		this->removeClient(fd);
	}
	else if (byte_read < 0)
	{
		perror("recv");
		std::cerr << "Error reading from client socket." << std::endl;
		this->removeClient(fd);
	}
	else
	{
		current_client->updateLastActivity();
		std::cout << "Received " << byte_read << " bytes from client fd: " << fd << std::endl;
		current_client->appendToBuffer(std::string(buffer, byte_read));
		std::cout << "Client buffer after read: " << current_client->getBuffer() << std::endl;
		if (current_client->isRequestComplete())
		{
			std::cout << "Request complete for client fd: " << fd << std::endl;
			setRealServConfig(*current_client, hadHostHeader(current_client->getBuffer()));
			this->_poll_fds[this->getIndexPollFd(fd)].events = POLLOUT;
			buffer[0] = '\0';
			beforeRequest(*current_client, current_client->getResponseInstance());
		}
	}
	return ;
}

// Handle writing data to the client
// This function checks if the client's buffer is empty, sends the data to the client, and updates the poll_fds to switch back to read mode.
void Server::handleClientWrite(int fd)
{
	Client *current_Client = this->getClient(fd);
	ssize_t byte_sent = 0;
	
	if (current_Client->getBuffer().empty())
	{
		std::cerr << "No data to send to client fd: " << fd << std::endl;
		return;
	}
	std::string response = current_Client->getResponseInstance().getResponse();
	std::cout << "Sending response to client fd: " << fd << "\nResponse:\n" << response << std::endl;
	byte_sent = send(fd, response.c_str(), response.size(), 0);
	if (byte_sent < 0)
	{
		perror("send");
		std::cerr << "Error sending data to client fd: " << fd << std::endl;
		this->removeClient(fd);
		return;
	}
	else if (byte_sent == 0)
	{
		std::cerr << "Client fd: " << fd << " closed connection." << std::endl;
		this->removeClient(fd);
		return;
	}
	else
	{
		current_Client->updateLastActivity();
		std::cout << "Sent " << byte_sent << " bytes to client fd: " << fd << std::endl;
		current_Client->clearBuffer();
		current_Client->clearResponseInstance();
		this->_poll_fds[this->getIndexPollFd(fd)].events = POLLIN; // Switch back to read mode
	}
	return ;
}

// Accept a new client connection
// This function accepts a new client connection, sets the socket to non-blocking mode, and adds the client to the list of clients and poll_fds.
void Server::acceptNewClient(int index)
{
	sockaddr_in client_address;
	socklen_t client_address_len = sizeof(client_address);

	int server_fd = this->_poll_fds[index].fd;

	
	int new_client_socket = accept(this->_servs[index].getListenFd(), (struct sockaddr *)&client_address, &client_address_len);
	if (new_client_socket < 0)
	{
		std::cerr << "Error accepting new connection" << std::endl;
		return;
	}
	Serv_config* serv_conf = NULL;
    for (size_t i = 0; i < this->_servs.size(); ++i)
    {
        if (this->_servs[i].getListenFd() == server_fd && this->_servs[i].getServName() == this->_servs[index].getServName())
        {
            serv_conf = &this->_servs[i];
            break;
        } 
    }
    if (!serv_conf)
    {
        std::cerr << "Error: no Serv_config found for server_fd" << std::endl;
        close(new_client_socket);
        return;
    }
	std::cout << "New client connected: " << new_client_socket << std::endl;
	this->setClientNonBlocking(new_client_socket);
	this->addClientToPollFds(new_client_socket, client_address, serv_conf);
}

// Remove a client from the server
//This function closes the socket, clears the client's buffer, marks it as closed, and removes it from the list of clients and poll_fds.
void Server::removeClient(int fd)
{
	std::cout << "Removing client with fd: " << fd << std::endl;
	close(fd);
	for (size_t i = 0; i < this->_clients.size(); ++i)
	{
		if (this->_clients[i]->getFd() == fd)
		{
			this->_clients[i]->clearBuffer();
			this->_clients[i]->setClosed(true);
			delete this->_clients[i];
			this->_clients.erase(this->_clients.begin() + i);
			break;
		}
	}
	for (size_t i = 0; i < this->_poll_fds.size(); ++i)
	{
		if (this->_poll_fds[i].fd == fd)
		{
			this->_poll_fds.erase(this->_poll_fds.begin() + i);
			break;
		}
	}
}

// Check for timeouts and remove clients that have been inactive for too long
void Server::checkTimeouts()
{
	time_t now;
	for (size_t i = 0; i < this->_clients.size(); ++i)
	{
		std::time(&now);
		if (std::difftime(now, this->_clients[i]->getLastActivity()) >= this->_clients[i]->getServConfig()->getTimeout())
		{
			std::cout << "Client fd: " << this->_clients[i]->getFd() << " timed out." << std::endl;
			this->removeClient(this->_clients[i]->getFd());
			--i;
		}
	}
}

// Set a client socket to non-blocking mode
void Server::setClientNonBlocking(int fd)
{
	if (fcntl(fd, F_SETFL, O_NONBLOCK) < 0)
	{
		perror("fcntl F_SETFL");
		close(fd);
		return;
	}
}

// Add a new client to the poll_fds and clients vector
void Server::addClientToPollFds(int new_client_socket, sockaddr_in &client_address, Serv_config *serv_conf)
{
	struct pollfd new_client_poll_fd;
	new_client_poll_fd.fd = new_client_socket;
	new_client_poll_fd.events = POLLIN;
	new_client_poll_fd.revents = 0;
	Client* new_client = new Client(new_client_socket, serv_conf->getPort(), ntohl(client_address.sin_addr.s_addr), serv_conf);
	this->_clients.push_back(new_client);
	this->_poll_fds.push_back(new_client_poll_fd);
	std::cout << "New client added to poll_fds with fd: " << new_client_socket << std::endl;
}


bool Server::checkIfServeSocketAlreadyExists(std::vector<Serv_config> &servs, int index)
{
	for (size_t i = 0; i < servs.size(); ++i)
	{
		if (i != static_cast<size_t>(index) && servs[i].getPort() == servs[index].getPort() && servs[i].getIp() == servs[index].getIp())
		{
			return true;
		}
	}
	return false;
}

std::string Server::hadHostHeader(std::string const &buffer)
{
    size_t pos = buffer.find("Host:");
    if (pos == std::string::npos)
        return "";
    
    size_t start = pos + 5; // Length of "Host:"
    size_t end = buffer.find("\r\n", start);
    if (end == std::string::npos)
        end = buffer.length();

    std::string host = buffer.substr(start, end - start);

    // Trim whitespace
    size_t first = host.find_first_not_of(" \t");
    size_t last = host.find_last_not_of(" \t");
    if (first == std::string::npos || last == std::string::npos)
        return "";

    host = host.substr(first, last - first + 1);

    // Supprimer le port s'il y en a
    size_t colon = host.find(':');
    if (colon != std::string::npos)
        host = host.substr(0, colon);

    return host;
}

void Server::setRealServConfig(Client &client, std::string const &host)
{
    Serv_config* matched = client.getServConfig(); // déjà initialisé à acceptNewClient()
    
    if (!host.empty())
    {
        for (size_t i = 0; i < _servs.size(); ++i)
        {
            if (_servs[i].getListenFd() == matched->getListenFd() &&
                _servs[i].getServName() == host)
            {
                matched = &_servs[i];
                break;
            }
        }
    }

    client.setServConfig(matched);
}