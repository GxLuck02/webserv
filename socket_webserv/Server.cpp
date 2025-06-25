/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttreichl <ttreichl@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 18:26:59 by ttreichl          #+#    #+#             */
/*   Updated: 2025/06/25 19:24:42 by ttreichl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

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
	address.sin_addr.s_addr =INADDR_ANY;
	address.sin_port = htons(this->_serv_config.getPort());
	if  (bind(this->_serv_socket, (struct sockaddr *)&address, sizeof(address)) < 0)
	{
		throw std::runtime_error("Error: error in bind of serversocket .");
		return ;
	}
	std::cout << "SOMAXCONN = " << SOMAXCONN << std::endl;
	if (listen(this->_serv_socket, SOMAXCONN) < 0)
	{
		throw std::runtime_error("Error: error in listen of serversocket .");
		return ;
	}
	std::cout << "Server initialized on port " << this->_serv_config.getPort() << std::endl;
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
		int poll_count = poll(this->_poll_fds.data(), this->_poll_fds.size(), -1);
		if (poll_count < 0)
		{
			std::cerr << "Error in poll" << std::endl;
			break;
		}

		for (size_t i = 0; i < this->_poll_fds.size(); ++i)
		{
			if (this->_poll_fds[i].revents & POLLIN)
			{
				if (this->_poll_fds[i].fd == this->_serv_socket)
				{
					this->acceptNewClient();
				}
				else
				{
	
					std::cout << "Data available on client socket: " << this->_poll_fds[i].fd << std::endl;
					// Read data from the client
				}
			}
		}
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

	this->_poll_fds.push_back(new_client_poll_fd);
}
