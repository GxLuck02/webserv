/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttreichl <ttreichl@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 18:26:59 by ttreichl          #+#    #+#             */
/*   Updated: 2025/07/02 19:26:36 by ttreichl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <unistd.h>
#include <arpa/inet.h>

Server::Server()
{
	std::cout << "Server default constructor called." << std::endl;
	this->_serv_socket = -1;
}

Server::~Server()
{
	if (this->_serv_socket != -1)
	{
		close(this->_serv_socket);
		std::cout << "Server socket closed." << std::endl;
	}
	for (size_t i = 0; i < this->_clients.size(); ++i)
	{
		if(this->_clients[i]->getFd() != -1)
		{
			close(this->_clients[i]->getFd());
			std::cout << "Closing client fd: " << this->_clients[i]->getFd() << std::endl;
		}
		delete this->_clients[i];
	}
	this->_poll_fds.clear();
	std::cout << "Server destructor called." << std::endl;
}

Server::Server(const Server &other)
	: _serv_config(other._serv_config), _serv_socket(other._serv_socket),
	  _clients(other._clients), _poll_fds(other._poll_fds)
{
	std::cout << "Server copy constructor called." << std::endl;
}

Server &Server::operator=(const Server &other)
{
	if (this != &other)
	{
		this->_serv_config = other._serv_config;
		this->_serv_socket = other._serv_socket;
		this->_clients = other._clients;
		this->_poll_fds = other._poll_fds;
		std::cout << "Server assignment operator called." << std::endl;
	}
	return *this;
}

/************************** Geters and Setters ***************************/

// Get the client object associated with a given file descriptor
Client *Server::getClient(int fd) const
{
	for (size_t i = 0; i < this->_clients.size(); ++i)
	{
		if (this->_clients[i]->getFd() == fd)
		{
			return this->_clients[i];
		}
	}
	throw std::runtime_error("Error: Client not found.");
}

// Get the index of the poll file descriptor for a given file descriptor
int Server::getIndexPollFd(int fd) const
{
	for (size_t i = 0; i < this->_poll_fds.size(); ++i)
	{
		if (this->_poll_fds[i].fd == fd)
		{
			return i;
		}
	}
	throw std::runtime_error("Error: Poll fd not found.");
}

/*************************** Gesture of Server ***************************/

// Initialize the server
// This function creates a socket, sets it to non-blocking mode, binds it to the specified IP and port, and starts listening for incoming connections.
void	Server::initServer()
{
	this->_serv_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_serv_socket < 0)
		throw std::runtime_error("Error: error in creation of serversocket.");
	int opt = 1;
	if (setsockopt(this->_serv_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) 
	{
		close(this->_serv_socket);
		throw std::runtime_error("Error: setsockopt failed.");
	}
	this->setSocketNonBlocking();
	this->bindSocket();
	this->listenSocket();
}

// Run the server
// This function initializes the server, sets up the poll file descriptors, and enters the main loop
void Server::run()
{
	std::cout << "Server is running..." << std::endl;
	struct pollfd serv_poll_fd;
	this->_poll_fds.clear();
	serv_poll_fd.fd = this->_serv_socket;
	serv_poll_fd.events = POLLIN;
	serv_poll_fd.revents = 0;
	this->_poll_fds.push_back(serv_poll_fd);
	while (true)
	{
		this->checkTimeouts();
		int poll_count = poll(this->_poll_fds.data(), this->_poll_fds.size(), 900);
		if (poll_count < 0)
		{
			throw std::runtime_error("Error: poll failed.");
		}
		if (this->_poll_fds[0].revents & POLLIN)
			this->acceptNewClient();
		for (long unsigned int i = 0; i < this->_poll_fds.size(); ++i)
		{
			if (this->_poll_fds[i].revents & POLLIN && this->_poll_fds[i].fd != this->_serv_socket)
			{
				std::cout << "Handling read for client fd: " << _poll_fds[i].fd << std::endl;
				this->handleClientRead(_poll_fds[i].fd);
			}
			if (this->_poll_fds[i].revents & POLLOUT)
			{
				std::cout << "Handling write for client fd: " << _poll_fds[i].fd << std::endl;
				this->handleClientWrite(_poll_fds[i].fd);
			}
			if (this->_poll_fds[i].revents & (POLLHUP | POLLERR | POLLNVAL))
			{
				std::cerr << "Error or hangup on client fd: " << this->_poll_fds[i].fd << std::endl;
				this->removeClient(this->_poll_fds[i].fd);
			}
			this->_poll_fds[i].revents = 0;
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
	if (this->_serv_socket != -1)
	{
		close(this->_serv_socket);
		this->_serv_socket = -1;
		std::cout << "Server socket closed." << std::endl;
	}
}
// Set the server socket to non-blocking mode
// This function uses fcntl to set the socket to non-blocking mode, allowing the server to handle multiple clients without blocking on read/write operations.
void Server::setSocketNonBlocking()
{
	int flags = fcntl(this->_serv_socket, F_GETFL, 0);
	if (flags < 0)
	{
		close(this->_serv_socket);
		throw std::runtime_error("Error: fcntl F_GETFL failed.");
	}
	if (fcntl(this->_serv_socket, F_SETFL, flags | O_NONBLOCK) < 0)
	{
		close(this->_serv_socket);
		throw std::runtime_error("Error: fcntl F_SETFL failed.");
	}
	std::cout << "Server socket set to non-blocking mode." << std::endl;
}

// Bind the server socket to the specified IP and port
// This function creates a sockaddr_in structure with the server's IP and port, and binds the socket to it.
void Server::bindSocket()
{
	sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = this->_serv_config.getIp();
	address.sin_port = htons(this->_serv_config.getPort());
	if  (bind(this->_serv_socket, (struct sockaddr *)&address, sizeof(address)) < 0)
	{
		close(this->_serv_socket);
		throw std::runtime_error("Error: error in bind of serversocket .");
	}
	std::cout << "Server initialized on port " << this->_serv_config.getPort() << std::endl;
	std::cout << "Server IP: " << inet_ntoa(address.sin_addr) << std::endl;
}

// Listen for incoming connections on the server socket
// This function sets the server socket to listen for incoming connections with a maximum queue size of SOM
void Server::listenSocket()
{
	if (listen(this->_serv_socket, SOMAXCONN) < 0)
	{
		close(this->_serv_socket);
		throw std::runtime_error("Error: error in listen of serversocket .");
	}
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
			if (errno == EAGAIN || errno == EWOULDBLOCK)
				return ;
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
				this->_poll_fds[this->getIndexPollFd(fd)].events = POLLOUT;
				buffer[0] = '\0';
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
	byte_sent = send(fd, current_Client->getBuffer().c_str(), current_Client->getBuffer().size(), 0);
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
		this->_poll_fds[this->getIndexPollFd(fd)].events = POLLIN; // Switch back to read mode
	}
	return ;
}

// Accept a new client connection
// This function accepts a new client connection, sets the socket to non-blocking mode, and adds the client to the list of clients and poll_fds.
void Server::acceptNewClient()
{
	sockaddr_in client_address;
	socklen_t client_address_len = sizeof(client_address);
	int new_client_socket = accept(this->_serv_socket, (struct sockaddr *)&client_address, &client_address_len);
	if (new_client_socket < 0)
	{
		std::cerr << "Error accepting new connection" << std::endl;
		return;
	}
	std::cout << "New client connected: " << new_client_socket << std::endl;
	this->setClientNonBlocking(new_client_socket);
	this->addClientToPollFds(new_client_socket, client_address);
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
			this->_clients[i]->clearBuffer(); // Clear buffer before deleting
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
		if (std::difftime(now, this->_clients[i]->getLastActivity()) >= this->_serv_config.getTimeout())
		{
			std::cout << "Client fd: " << this->_clients[i]->getFd() << " timed out." << std::endl;
			this->removeClient(this->_clients[i]->getFd());
			--i;
		}
	}
}

void Server::setClientNonBlocking(int fd)
{
	int flags = fcntl(fd, F_GETFL, 0);
	if (flags < 0)
	{
		perror("fcntl F_GETFL");
		close(fd);
		return;
	}
	if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) < 0)
	{
		perror("fcntl F_SETFL");
		close(fd);
		return;
	}
}

void Server::addClientToPollFds(int new_client_socket, sockaddr_in &client_address)
{
	struct pollfd new_client_poll_fd;
	new_client_poll_fd.fd = new_client_socket;
	new_client_poll_fd.events = POLLIN;
	new_client_poll_fd.revents = 0;
	Client* new_client = new Client(new_client_socket, this->_serv_config.getPort(), ntohl(client_address.sin_addr.s_addr));
	this->_clients.push_back(new_client);
	this->_poll_fds.push_back(new_client_poll_fd);
	std::cout << "New client added to poll_fds with fd: " << new_client_socket << std::endl;
}
