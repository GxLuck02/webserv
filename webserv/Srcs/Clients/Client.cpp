/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttreichl <ttreichl@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 17:11:22 by ttreichl          #+#    #+#             */
/*   Updated: 2025/08/06 23:06:41 by ttreichl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include <algorithm>

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
	std::cout << "[ Checking if request is complete ]" << std::endl;
    std::string line_ending = "\r\n";
    std::string header_separator = "\r\n\r\n";
    
    size_t header_end = _buffer.find(header_separator);
    if (header_end == std::string::npos)
	{
		std::cout << "[ Header not found ]" << std::endl;
        return false;
	}

    size_t content_length_pos = _buffer.find("Content-Length:");
    if (content_length_pos != std::string::npos)
    {
		std::cout << "[ Cheking Content-Length ]" << std::endl;
        // Trouver le début de la valeur après ':'
        size_t value_start = content_length_pos + 15; // longueur de "content-length:"
        
        // Ignorer les espaces dans le buffer original (pas lowercase)
        while (value_start < header_end && std::isspace(_buffer[value_start]))
            ++value_start;
           
        // Trouver la fin de la ligne
        size_t value_end = _buffer.find(line_ending, value_start);
		std::cout << "[ Value Start ] : " << value_start << ", [ Value End ] : " << value_end << std::endl;
        if (value_end != std::string::npos)
        {
            std::string value_str = _buffer.substr(value_start, value_end - value_start);
			std::cout << "[ Content-Length Value ] : " << value_str << std::endl;
            
           size_t len_str = atoi(value_str.c_str());
		   std::string body_buffer = _buffer.substr(header_end + 4, _buffer.size() - header_end - 4);
		   			std::cout << "[ Body Buffer ] : " << "/ " << body_buffer << "/ " << std::endl;
					std::cout << "[ Body Buffer Size ] : " << body_buffer.size() << std::endl;
            
            // Validation plus robuste du nombre
            try {
                size_t content_length = len_str;
				std::cout << "[ Content-Length ] : " << content_length << std::endl;
                size_t total_expected_length = header_end + 6 + content_length;
				std::cout << "[ Total Expected Length ] : " << total_expected_length << std::endl;
				std::cout << "[ Buffer Size ] : " << _buffer.size() << std::endl;
                return _buffer.size() >= total_expected_length;
            }
            catch (const std::exception&) {
                // Content-Length invalide
				std::cout << "[ Content-Length invalid ]" << std::endl;
                return false;
            }
        }
		std::cout << "[ Content-Length malformed ]" << std::endl;
        return false; // Content-Length mal formé
    }
	
    // // Vérifier Transfer-Encoding: chunked (case-insensitive)
    // if (_buffer.find("Transfer-Encoding:") != std::string::npos)
    // {
	// 	std::cout << "[ Checking Transfer-Encoding ]" << std::endl;
	// 	// Trouver la position de "transfer-encoding:"
	// 	size_t te_pos = _buffer.find("Transfer-Encoding:");
    //     size_t te_end = _buffer.find(line_ending, te_pos);
    //     if (te_end != std::string::npos)
    //     {
    //         std::string te_value = _buffer.substr(te_pos + 18, te_end - te_pos - 18);
    //         if (te_value.find("chunked") != std::string::npos)
    //         {
    //             // Pour chunked, chercher la séquence de fin
    //             std::string chunk_terminator = "0\r\n\r\n";
    //             size_t search_start = header_end + header_separator.length();
    //             size_t terminator_pos = _buffer.find(chunk_terminator, search_start);
    //             return (terminator_pos != std::string::npos);
    //         }
    //     }
    // }
    std::string method = _buffer.substr(0, _buffer.find(' '));
    std::transform(method.begin(), method.end(), method.begin(), ::toupper);
	std::cout << "[ Cheking Method ] :" << method << std::endl;
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