/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server_configue.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttreichl <ttreichl@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 17:47:14 by ttreichl          #+#    #+#             */
/*   Updated: 2025/06/25 19:14:00 by ttreichl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server_configue.hpp"

Serv_config::Serv_config()
{
	this->_servName = "";
	this->_port = 8080;
	this->_ip = "127.0.0.1";
	this->_max_clients = 0;
	this->_timeout = 0;
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
		std::cout << "Serv_config assignment operator called." << std::endl;
	}
	return *this;
}

/**************************** Members functions ************************************/

const std::string &Serv_config::getServName() const
{
	return this->_servName;
}

int Serv_config::getPort() const
{
	return this->_port;
}

const std::string &Serv_config::getIp() const
{
	return this->_ip;
}

int Serv_config::getMaxClients() const
{
	return this->_max_clients;
}

int Serv_config::getTimeout() const
{
	return this->_timeout;
}

