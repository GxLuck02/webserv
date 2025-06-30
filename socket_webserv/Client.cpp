/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttreichl <ttreichl@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 17:11:22 by ttreichl          #+#    #+#             */
/*   Updated: 2025/06/30 18:12:48 by ttreichl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client(int fd, int port, in_addr_t ip)
	: _fd(fd), _port(port), _ip(ip), _isClosed(false), _lastActivity(time(NULL))
{
	this->_buffer.clear();
	std::cout << "Client created with fd: " << _fd << ", port: " << _port << std::endl;
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
	this->appendToBuffer(other.getBuffer());
	std::cout << "Client copied with fd: " << _fd << std::endl;
}

Client &Client::operator=(const Client &other)
{
	if (this != &other)
	{
		// Do not copy file descriptor, as it should be unique per client
		// _fd = other._fd; // Removed to avoid double-close and resource issues
		_port = other._port;
		_ip = other._ip;
		_isClosed = other._isClosed;
		this->appendToBuffer(other.getBuffer());
		std::cout << "Client assigned (fd not copied) with fd: " << _fd << std::endl;
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

in_addr_t Client::getIp() const
{
	 return(this->_ip);
}

const std::string &Client::getBuffer() const
{
	 return(const_cast<std::string &>(_buffer));
}

void Client::appendToBuffer(const std::string& data)
{
	_buffer += data;
}
void Client::clearBuffer()
{
	_buffer.clear();
}

bool Client::isRequestComplete() const
{
	// just for simple test, check after for all requests
	return _buffer.find("\r\n\r\n") != std::string::npos;
}

bool Client::isClosed() const
{
	return _isClosed;
}

void Client::setClosed(bool closed){
	_isClosed = closed;
	if (closed && _fd != -1) {
		close(_fd);
	}
}

void Client::updateLastActivity()
{
	_lastActivity = time(NULL);
}