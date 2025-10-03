/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttreichl <ttreichl@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 17:11:22 by ttreichl          #+#    #+#             */
/*   Updated: 2025/10/03 15:58:32 by ttreichl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include <algorithm>
#include <list>

Client::Client(int fd, int port, in_addr_t ip, Serv_config* serv_config)
    : _serv_config(serv_config), _fd(fd), _port(port), _ip(ip), _isClosed(false), _lastActivity(time(NULL)), _isError(false)
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
	: _fd(other._fd), _port(other._port), _ip(other._ip), _isClosed(other._isClosed), _isError(other._isError)    
{
	this->appendToBuffer(other.getBuffer());
	std::cout << "Client copied with fd: " << _fd << std::endl;
}

Client &Client::operator=(const Client &other)
{
	if (this != &other)
	{
        _serv_config = other._serv_config;
        _fd = other._fd;
		_port = other._port;
		_ip = other._ip;
		_isClosed = other._isClosed;
        _isError = other._isError;
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

void Client::setServConfig(Serv_config* serv_config)
{
    this->_serv_config = serv_config;
}

bool Client::getErrorFlag() const
{
    return this->_isError;
}

void Client::setErrorFlag(bool flag)
{
    this->_isError = flag;
}

/************************************ Members functions ************************************/

//append string at the buffer
void Client::appendToBuffer(const std::string& data)
{
	_buffer += data;
}

void Client::clearBuffer()
{
	_buffer = "";
}

// Check if the request is complete by looking for the end of the HTTP header
bool Client::isRequestComplete()
{
    std::string line_ending = "\r\n";
    std::string header_separator = "\r\n\r\n" ; /*\r\n\r\n*/
    std::cout << "Checking if request is complete for client fd: " << this->getFd() << std::endl;
    std::cout << "Max body size: " << this->getServConfig()->getMaxBodySize() << std::endl;
    
    std::cout << "Current buffer size: " << _buffer.size() << std::endl;
    std::cout << "Current buffer content:\n" << _buffer << std::endl;
    size_t header_end = _buffer.find(header_separator);
    if (header_end == std::string::npos)
    {
        std::cout << "Header end not found." << std::endl;
        return false;
    }
    size_t transfer_encoding_pos = _buffer.find("Transfer-Encoding:");
    std::string transfer_encoding;

    if (transfer_encoding_pos != std::string::npos)
    {
        size_t te_end = _buffer.find("\r\n", transfer_encoding_pos);
        if (te_end != std::string::npos)
        {
            transfer_encoding = _buffer.substr(transfer_encoding_pos + 18,
                                              te_end - (transfer_encoding_pos + 18));

            // trim début
            while (!transfer_encoding.empty() &&
                  (transfer_encoding[0] == ' ' || transfer_encoding[0] == '\t'))
            {
                transfer_encoding.erase(0, 1);
            }

            // trim fin
            while (!transfer_encoding.empty() &&
                  (transfer_encoding[transfer_encoding.size() - 1] == ' ' ||
                    transfer_encoding[transfer_encoding.size() - 1] == '\t'))
            {
                transfer_encoding.erase(transfer_encoding.size() - 1, 1);
            }

            // lowercase (style C++98)
            for (size_t i = 0; i < transfer_encoding.size(); ++i)
            {
                transfer_encoding[i] = static_cast<char>(std::tolower(
                    static_cast<unsigned char>(transfer_encoding[i])));
            }
        }
    }

    // Vérifie si "chunked" apparaît
    if (transfer_encoding.find("chunked") != std::string::npos)
    {
        if (checkTransferEncodingChunked())
            return true;
        else
            return false;
    }
    size_t content_length_pos = _buffer.find("Content-Length:");
    std::cout << "Content-Length position: " << content_length_pos << std::endl;
    if (content_length_pos != std::string::npos)
    {
        size_t value_start = content_length_pos + 15;
        while (value_start < header_end && std::isspace(_buffer[value_start]))
            ++value_start;
        size_t value_end = _buffer.find(line_ending, value_start);
        if (value_end != std::string::npos)
        {
            std::string value_str = _buffer.substr(value_start, value_end - value_start);
            size_t len_str = atoi(value_str.c_str());
		    std::string body_buffer = _buffer.substr(header_end + 4, _buffer.size() - header_end - 4);
            try {
                size_t content_length = len_str;
                if (body_buffer.size() > static_cast<size_t>(this->getServConfig()->getMaxBodySize()))
                {
                    std::cout << "Content-Length: " << body_buffer.size() << std::endl;
                    this->_isError = true;
                }
                size_t total_expected_length = header_end + 4 + content_length;
                if (_buffer.size() >= total_expected_length)
                    return true;
            }
            catch (const std::exception&)
            {
                return false;
            }
        }
    }
    if (_isError == true)
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
    std::cout << "Checking for chunked transfer encoding " << std::endl;
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
                {
                    std::cout << "Chunked transfer encoding complete." << std::endl;
                    return true;
                }
                else
                {
                    std::cout << "Chunked transfer encoding not complete." << std::endl;
                    return false;
                }
            }
        }
    }
    std::cout << "No chunked transfer encoding found." << std::endl;
    return false;
}

bool Client::searchChunkedEnd(size_t header_end) const
{
    if (header_end + 4 >= _buffer.size())
        return false;

    size_t pos = header_end + 4;

    while (pos < _buffer.size())
    {
        // chercher la fin de la ligne (chunk-size)
        size_t crlf = _buffer.find("\r\n", pos);
        if (crlf == std::string::npos)
            return false;

        // récupérer la taille du chunk en hex
        std::string size_str = _buffer.substr(pos, crlf - pos);
        char *endptr;
        long chunk_size = std::strtol(size_str.c_str(), &endptr, 16);

        if (endptr == size_str.c_str() || chunk_size < 0)
            return false;

        pos = crlf + 2; // avancer après la ligne de taille

        if (chunk_size == 0)
        {
            // vérifier la séquence de fin "\r\n" qui suit le dernier chunk
            if (pos + 2 <= _buffer.size() && _buffer.substr(pos, 2) == "\r\n")
                return true;
            return false;
        }

        // vérifier que le chunk complet est présent
        if (pos + static_cast<size_t>(chunk_size) + 2 > _buffer.size())
            return false;

        pos += chunk_size + 2; // avancer après les données et leur \r\n
    }

    return false;
}


void Client::clearResponseInstance()
{
    this->_responseInstance = Response();
}

// Get the server configuration for the client
Serv_config* Client::getServConfig() const
{
	return this->_serv_config;
}