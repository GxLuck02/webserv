/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server_configue.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttreichl <ttreichl@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 17:47:14 by ttreichl          #+#    #+#             */
/*   Updated: 2025/07/11 17:04:21 by ttreichl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server_configue.hpp"

Serv_config::Serv_config()
{
	this->_servName = "";
	this->_port = 0;
	this->_ip = 0;
	this->_max_clients = 0;
	this->_timeout = 60;
	this->_listen_fd = -1;
	this->_max_body_size = 1000000;
	std::cout << "Serv_config default constructor called." << std::endl;
}

Serv_config::~Serv_config()
{
	std::cout << "Serv_config destructor called." << std::endl;
}

Serv_config::Serv_config(const Serv_config &other)
	: _servName(other._servName), _port(other._port), _ip(other._ip),
	  _max_clients(other._max_clients), _timeout(other._timeout)
{
	std::cout << "Serv_config copy constructor called." << std::endl;
}

Serv_config &Serv_config::operator=(const Serv_config &other)
{
	if (this != &other)
	{
		this->_servName = other._servName;
		this->_port = other._port;
		this->_ip = other._ip;
		this->_max_clients = other._max_clients;
		this->_timeout = other._timeout;
		this->_listen_fd = other._listen_fd;
		std::cout << "Serv_config assignment operator called." << std::endl;
	}
	return *this;
}

/**************************** Geters and Setters ***********************************/

void Serv_config::setPort(int port)
{
	if (port < 0 || port > 65535)
	{
		std::cerr << "Error: Invalid port number. Must be between 0 and 65535." << std::endl;
		return;
	}
	this->_port = port;
}

int Serv_config::getPort() const
{
	return this->_port;
}

void Serv_config::setIp()
{
	this->_ip = inet_addr("127.0.0.1");
}

int Serv_config::getIp() const
{
	return this->_ip;
}

void Serv_config::setTimeout(int timeout)
{
	if (timeout < 0)
	{
		std::cerr << "Error: Timeout must be a positive integer." << std::endl;
		return;
	}
	this->_timeout = timeout;
}

int Serv_config::getTimeout() const
{
	return this->_timeout;
}

void Serv_config::setServName(const std::string &servName)
{
	if (servName.empty())
	{
		std::cerr << "Error: Server name cannot be empty." << std::endl;
		return;
	}
	this->_servName = servName;
}

const std::string &Serv_config::getServName() const
{
	return this->_servName;
}

void Serv_config::setListenFd(int listen_fd)
{
	if (listen_fd < 0)
	{
		std::cerr << "Error: Listen file descriptor must be a non-negative integer." << std::endl;
		return;
	}
	this->_listen_fd = listen_fd;
}

int Serv_config::getListenFd() const
{
	return this->_listen_fd;
}

/**************************** Members functions ************************************/

