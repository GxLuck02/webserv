/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: proton <proton@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 17:11:22 by ttreichl          #+#    #+#             */
/*   Updated: 2025/09/18 17:41:58 by proton           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include <algorithm>
#include <list>

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

void Client::setClosed(bool closed)
{
	_isClosed = closed;
	if (closed && _fd != -1)
		close(_fd);
}

Response& Client::getResponseInstance() const
{
    return (const_cast<Response &>(_responseInstance));
}

void Client::setResponseInstance(const Response& responseInstance)
{
    this->_responseInstance = responseInstance;
}

/************************************ Members functions ************************************/

//append string at the buffer
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
    std::string line_ending = "\r\n";
    std::string header_separator = "\r\n\r\n";
    
    size_t header_end = _buffer.find(header_separator);
    if (header_end == std::string::npos)
        return false;
    size_t content_length_pos = _buffer.find("Content-Length:");
    if (content_length_pos != std::string::npos)
    {
        size_t value_start = content_length_pos + 15;
        while (value_start < header_end && std::isspace(_buffer[value_start]))
            ++value_start;
        size_t value_end = _buffer.find(line_ending, value_start);
        if (value_end != std::string::npos)
        {
            std::string value_str = _buffer.substr(value_start, value_end - value_start);
			std::cout << "[ Content-Length Value ] : " << value_str << std::endl;
            std::cout << "[ actual buffer size ] : " << _buffer.size() - (header_end + 4) << std::endl;
            
            size_t len_str = atoi(value_str.c_str());
		    std::string body_buffer = _buffer.substr(header_end + 4, _buffer.size() - header_end - 4);
            try {
                size_t content_length = len_str;
                size_t total_expected_length = header_end + 4 + content_length;
                return _buffer.size() >= total_expected_length;
            }
            catch (const std::exception&)
            {
                return false;
            }
        }
        return false;
    }
    if (checkTransferEncodingChunked())
          return true;
    std::string method = _buffer.substr(0, _buffer.find(' '));
    std::transform(method.begin(), method.end(), method.begin(), ::toupper);
    if (method == "GET" || method == "HEAD" || method == "DELETE" || method == "OPTIONS")
        return true;
    if (method == "POST" || method == "PUT" || method == "PATCH")
        return false;
    return true;
}

// Update the last activity time of the client
void Client::updateLastActivity()
{
	_lastActivity = time(NULL);
}

bool Client::checkTransferEncodingChunked() const
{
    size_t header_end = _buffer.find("\r\n\r\n");
    if (header_end == std::string::npos)
        return false;
    if (_buffer.find("Transfer-Encoding:") != std::string::npos)
    {
		size_t te_pos = _buffer.find("Transfer-Encoding:");
        size_t te_end = _buffer.find("\r\n", te_pos);
        if (te_end != std::string::npos)
        {
            std::string te_value = _buffer.substr(te_pos + 18, te_end - te_pos - 18);
            if (te_value.find("chunked") != std::string::npos)
            {  
                if (searchChunkedEnd(header_end))
                    return true;
                else
                    return false;
            }
        }
    }
    return false;
}

bool Client::searchChunkedEnd(size_t header_end) const
{
    if (header_end + 4 >= _buffer.size()) return false;
    size_t pos = header_end + 4;
    while (pos < _buffer.size())
    {
        size_t crlf = _buffer.find("\r\n", pos);
        if (crlf == std::string::npos)
            return false;
        std::string size_str = _buffer.substr(pos, crlf - pos - 4);
        bool valid_hex = true;
        for (size_t i = 0; i < size_str.size(); ++i)
        {
            if (!std::isxdigit(static_cast<unsigned char>(size_str[i])))
            {
                valid_hex = false;
                break;
            }
        }
        if (!valid_hex)
        {
            pos = crlf + 2;
            continue;
        }
        char *endptr;
        long chunk_size = std::strtol(size_str.c_str(), &endptr, 16);
        if (endptr == size_str.c_str() || chunk_size < 0)
            return false;
        if (chunk_size == 0)
            return true;
        if (pos + static_cast<size_t>(chunk_size) + 2 > _buffer.size())
            return false;
        pos += chunk_size + 2;
    }

    return false;
}
