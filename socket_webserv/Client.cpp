/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttreichl <ttreichl@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 17:11:22 by ttreichl          #+#    #+#             */
/*   Updated: 2025/06/25 17:32:48 by ttreichl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client(int fd, int port, int ip)
	: _fd(fd), _port(port), _ip(ip), _isClosed(false)
{
	std::cout << "Client created with fd: " << _fd << ", port: " << _port << ", ip: " << _ip << std::endl;
}

Client::~Client()
{
	if (!_isClosed)
	{
		close(_fd);
		_isClosed = true;
		std::cout << "Client with fd: " << _fd << " closed." << std::endl;
	}
}

Client::Client(const Client &other)
	: _fd(other._fd), _port(other._port), _ip(other._ip), _isClosed(other._isClosed)
{
	this->setBuffer(other.getBuffer());
	std::cout << "Client copied with fd: " << _fd << std::endl;
}

Client &Client::operator=(const Client &other)
{
	if (this != &other)
	{
		_fd = other._fd;
		_port = other._port;
		_ip = other._ip;
		_isClosed = other._isClosed;
		this->setBuffer(other.getBuffer());
		std::cout << "Client assigned with fd: " << _fd << std::endl;
	}
	return *this;
}

/************************************ Members functions ************************************/

int Client::getFd() const
{
	 return(this->_fd);
}

int Client::getPort() const
{
	 return(this->_port);
}

int Client::getIp() const
{
	 return(this->_ip);
}

const std::string &Client::getBuffer() const
{
	 return(const_cast<std::string &>(_buffer));
}

void Client::setBuffer(const std::string &data)
{
	if (data.empty())
	{
		std::cerr << "Error: Attempt to set an empty buffer." << std::endl;
		return;
	}
	_buffer = data;
}
