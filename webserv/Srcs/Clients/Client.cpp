/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttreichl <ttreichl@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 17:11:22 by ttreichl          #+#    #+#             */
/*   Updated: 2025/07/14 14:11:05 by ttreichl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client(int fd, int port, in_addr_t ip, Serv_config* serv_config)
	: _serv_config(serv_config), _fd(fd), _port(port), _ip(ip), _isClosed(false), _lastActivity(time(NULL))
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
		_port = other._port;
		_ip = other._ip;
		_isClosed = other._isClosed;
		this->appendToBuffer(other.getBuffer());
		std::cout << "Client assigned (fd not copied) with fd: " << _fd << std::endl;
	}
	return *this;
}

/************************************ Getters and Setters **********************************/


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

time_t Client::getLastActivity() const
{
	 return(this->_lastActivity);
}

const std::string &Client::getBuffer() const
{
	 return(const_cast<std::string &>(_buffer));
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

/************************************ Members functions ************************************/

void Client::appendToBuffer(const std::string& data)
{
	_buffer += data;
}
void Client::clearBuffer()
{
	_buffer.clear();
}

// Check if the request is complete by looking for the end of the HTTP header
bool Client::isRequestComplete() const
{
	// just for simple test, check after for all requests
	return _buffer.find("\r\n\r\n") != std::string::npos;
}

// Update the last activity time of the client
void Client::updateLastActivity()
{
	_lastActivity = time(NULL);
}