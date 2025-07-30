/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: proton <proton@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 12:27:02 by proton            #+#    #+#             */
/*   Updated: 2025/07/28 13:07:16 by proton           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "request.hpp"

Request::Request()
{
	std::cout << "request base constructor" << std::endl;
}

Request::Request( const Request& copy )
{
	std::cout << "request copy constructor" << std::endl;
	*this = copy;
}

Request::~Request()
{
	std::cout << "request destructor" << std::endl;
}

Request&	Request::operator=( const Request& copy )
{
	if (this != &copy)
	{
		this->_methode = copy._methode;
		this->_uri = copy._uri;
		this->_httpVersion = copy._httpVersion;

	}
	return (*this);
}

std::string	Request::getMethode() const
{
	return (this->_methode);
}

std::string	Request::getUri() const
{
	return (this->_uri);
}

std::string	Request::getHttpVersion() const
{
	return (this->_httpVersion);
}

std::string	Request::getField( size_t loopRound )
{
	std::map<std::string, std::string>::iterator	it = this->_field.begin();
	size_t						i = 0;
	
	it++;

	while (i < loopRound && it != this->_field.end())
	{
		it++;
		i++;
	}
	return (it->first);	
}

std::string	Request::getField( std::string key )
{
	std::map<std::string, std::string>::iterator it = this->_field.begin();
	if (key.empty())
		return (key);

	while (it != this->_field.end())
	{
		if (it->first == key)
			return (it->second);
		it++;
	}
	return (key);
}

std::string	Request::getBody( std::string key, enum e_map keyOrValue )
{
	std::map<std::string, std::string>::iterator it = this->_body.begin();

	while (it != this->_body.end())
	{
		if (it->first == key)
		{
			if (keyOrValue == KEY)
				return (it->first);
			else
				return (it->second);
		}
		it++;
	}
	return ("");
}

int	Request::getStatusCode() const
{
	return (this->_statusCode);
}

int	Request::getFieldLength()
{
	std::map<std::string, std::string>::iterator	it = this->_field.begin();
	size_t						i = 0;

	while (it != this->_field.end())
	{
		it++;
		i++;
	}
	
	return (i);
}

std::string	Request::getContentType() const
{
	return (this->_contentType);
}

int	Request::getContentLength() const
{
	return (this->_contentLength);
}

void	Request::setMethode( std::string methode )
{
	this->_methode = methode;
}

void	Request::setUri( std::string uri )
{
	this->_uri = uri;
}

void	Request::setHttpVersion( std::string httpVersion )
{
	this->_httpVersion = httpVersion;
}

void	Request::setStatusCode( int status )
{
	this->_statusCode = status;
}

void	Request::setField( std::string key, std::string value )
{
	if (key.empty() || value.empty())
		return ;
	this->_field.insert(std::pair<std::string, std::string>(key, value));
}

void	Request::setContentLength( int length )
{
	this->_contentLength = length;
}

void	Request::setContentType( std::string type )
{
	this->_contentType = type;
}

void	Request::setBody( std::string key, std::string value )
{
	if (key.empty() || value.empty())
		return ;
	this->_body.insert(std::pair<std::string, std::string>(key, value));
}

void	Request::eraseMap()
{
	std::map<std::string, std::string>::iterator	it;

	it = this->_field.begin();

	while (it != this->_field.end())
	{
		this->_field.erase(it);
		it++;
	}
}

