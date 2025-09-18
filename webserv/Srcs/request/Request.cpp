/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: proton <proton@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2025/09/18 17:44:35 by proton           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "Request.hpp"

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
	return ("");
}

std::string	Request::getErrorBody() const
{
	return (this->_errorBody);
}

std::string	Request::getBody( size_t index, enum e_map keyOrValue )
{
	std::map<std::string, std::string>::iterator it = this->_body.begin();

	for(size_t i = 0; i < index && it != this->_body.end(); i++, it++)
	{
		if (keyOrValue == KEY)
			return (it->first);
		else
			return (it->second);
	}
	return ("");
}

std::string	Request::getQuery() const
{
	return (this->_query);
}

size_t	Request::getBodyLength() const
{
	return (this->_body.size());
}

std::string	Request::getBodyStart() const
{
	return (this->_bodyStart);
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

int	Request::getChunked() const
{
	return (this->_chunked);
}

std::string	Request::getLocation() const
{
	return (this->_location);
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

void	Request::setErrorBody( std::string body )
{
	this->_errorBody = body;
}

void	Request::setBody( std::string key, std::string value )
{
	if (key.empty() || value.empty())
		return ;
	this->_body.insert(std::pair<std::string, std::string>(key, value));
}

void	Request::setBodyStart( std::string bodyStart )
{
	this->_bodyStart = bodyStart;
}

void	Request::setChunked( int chunked )
{
	this->_chunked = chunked;
}

void	Request::setQuery( std::string query )
{
	this->_query = query;
}

void	Request::setLocation( std::string location )
void	Request::setLocation( std::string location )
{
	this->_location = location;
}

void	Request::setAutoindexEntries( std::string &path )
{
	this->_autoIndexEntries.push_back(path);	
}

std::string Request::getAutoIndexEntries( size_t index )
{
	return (this->_autoIndexEntries[index]);
}

size_t	Request::getAutoIndexEntriesSize() const
{
	return (this->_autoIndexEntries.size());
}

void	Request::setIsFullPath( bool trueOrFalse )
{
	if (trueOrFalse == true)
	{
		this->_isFullPath = true;
		return ;
	}
	this->_isFullPath = false;
}

bool	Request::getIsFullPath() const
{
	return (this->_isFullPath);
}

void	Request::setIsAutoIndex( bool autoindex )
{
	this->_isAutoindex = autoindex;
}

bool	Request::getIsAutoIndex() const
{
	return (this->_isAutoindex);
}

void	Request::setIsStaticCgi( bool isStaticCgi )
{
	this->_isStaticCgi = isStaticCgi;
}
bool	Request::getIsStaticCgi() const
{
	return (this->_isStaticCgi);
}

