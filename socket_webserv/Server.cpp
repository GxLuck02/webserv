/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttreichl <ttreichl@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 18:26:59 by ttreichl          #+#    #+#             */
/*   Updated: 2025/07/02 17:06:34 by ttreichl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <unistd.h>
#include <arpa/inet.h>

// class Server
// {
// 	private:
// 		// Server configuration
// 		Serv_config _serv_config;
// 		int _serv_socket;
// 		std::vector<Client> _clients;
// 		std::vector<pollfd> _poll_fds; //permet de suivre les fds avec poll()
// 	public:
// 		Server();
// 		~Server();
// 		Server(const Server &other);
// 		Server &operator=(const Server &other);
		
// 		void initServer();
// 		void run();
// 		void acceptNewClient();
		
// };

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

/*************************** Members functions ***************************/

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

void	Server::initServer()
{
	this->_serv_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_serv_socket < 0)
	{
		throw std::runtime_error("Error: error in creation of serversocket .");
		return ;
	}
	int opt = 1;
  	if (setsockopt(this->_serv_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
			perror("setsockopt");
			std::cerr << "Error: setsockopt failed." << std::endl;
			close(this->_serv_socket);
			return ;
    }
	int flags = fcntl(this->_serv_socket, F_GETFL, 0);
	if (flags < 0)
	{
		perror("fcntl server F_GETFL");
		close(this->_serv_socket);
		return ;
	}
	if (fcntl(this->_serv_socket, F_SETFL, flags | O_NONBLOCK) < 0)
	{
		perror("fcntl server F_SETFL");
		close(this->_serv_socket);
		return ;
	}
	sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = this->_serv_config.getIp();
	address.sin_port = htons(this->_serv_config.getPort());
	if  (bind(this->_serv_socket, (struct sockaddr *)&address, sizeof(address)) < 0)
	{
		close(this->_serv_socket);
		throw std::runtime_error("Error: error in bind of serversocket .");
	}
	std::cout << "SOMAXCONN = " << SOMAXCONN << std::endl;
	if (listen(this->_serv_socket, SOMAXCONN) < 0)
		throw std::runtime_error("Error: error in listen of serversocket .");
	std::cout << "Server initialized on port " << this->_serv_config.getPort() << std::endl;
	std::cout << "Server IP: " << inet_ntoa(address.sin_addr) << std::endl;
}

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
			std::cerr << "Error in poll" << std::endl;
			break;
		}
		if (this->_poll_fds[0].revents & POLLIN)
		{
			this->acceptNewClient();
			
		}
		for (long unsigned int i = 0; i < this->_poll_fds.size(); ++i)
		{
			if (this->_poll_fds[i].revents & POLLIN && this->_poll_fds[i].fd != this->_serv_socket)
			{
				// Handle read event for client sockets
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
			this->_poll_fds[i].revents = 0; // Reset revents for next poll
		}
	}
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

	int flags = fcntl(new_client_socket, F_GETFL, 0);
	if (flags < 0)
	{
		perror("fcntl F_GETFL");
		close(new_client_socket);
		return;
	}
	if (fcntl(new_client_socket, F_SETFL, flags | O_NONBLOCK) < 0)
	{
		perror("fcntl F_SETFL");
		close(new_client_socket);
		return;
	}
	struct pollfd new_client_poll_fd;
	new_client_poll_fd.fd = new_client_socket;
	new_client_poll_fd.events = POLLIN;
	new_client_poll_fd.revents = 0;
	Client* new_client = new Client(new_client_socket, this->_serv_config.getPort(), ntohl(client_address.sin_addr.s_addr));
	this->_clients.push_back(new_client);
	this->_poll_fds.push_back(new_client_poll_fd);
}

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