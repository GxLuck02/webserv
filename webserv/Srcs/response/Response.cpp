/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttreichl <ttreichl@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 17:28:27 by proton            #+#    #+#             */
/*   Updated: 2025/09/05 16:48:52 by ttreichl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

Response::Response() : _httpVersion("HTTP/1.1")
{
	std::cout << "Response base constructor" << std::endl;
}

Response::Response( const Response& copy )
{
	std::cout << "Response copy constructor" << std::endl;
	*this = copy;
}

Response::~Response()
{
	std::cout << "Response destructor" << std::endl;
}

Response&	Response::operator=( const Response& copy )
{
	if (this != &copy)
	{
		this->_statusCode = copy._statusCode;
		this->_statusLine = copy._statusLine;
		this->_httpVersion = copy._httpVersion;
		this->_contentType = copy._contentType;
		this->_contentLength = copy._contentLength;
		this->_body = copy._body;
		this->_fileType = copy._fileType;
		this->_header = copy._header;
		this->_response = copy._response;
	}

	return (*this);
}

std::string	Response::getStatusLine() const
{
	return (this->_statusLine);
}

int	Response::getStatusCode() const
{
	return (this->_statusCode);
}

std::string	Response::getHttpVersion() const
{
	return (this->_httpVersion);
}

std::string	Response::getContentType() const
{
	return (this->_contentType);
}

std::string	Response::getContentLength() const
{
	return (this->_contentLength);
}

std::string	Response::getResponse() const
{
	return (this->_response);
}

std::string	Response::getBody() const
{
	return (this->_body);
}

std::string	Response::getFileType() const
{
	return (this->_fileType);
}

void	Response::setStatusLine( std::string statusLine )
{
	this->_statusLine = statusLine;
}

void	Response::setResponse( std::string response )
{
	if (response.empty())
		return ;
	this->_response = response;
}

void	Response::setStatusCode( int status )
{
	this->_statusCode = status;
}

void	Response::setContentType( std::string type )
{
	this->_contentType = type;
}

void	Response::setContentLength( int length )
{
	this->_contentLength = length;
}

void	Response::setBody( std::string body )
{
	this->_body = body;
}

void	Response::setFileType( std::string fileType)
{
	this->_fileType = fileType;
}
