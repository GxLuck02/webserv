/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server_configue.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttreichl <ttreichl@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 17:47:14 by ttreichl          #+#    #+#             */
/*   Updated: 2025/08/25 15:55:20 by ttreichl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server_configue.hpp"
#include "Define.hpp"

Serv_config::Serv_config()
{
	std::cout << "Serv_config constructor called." << std::endl;
}

Serv_config::~Serv_config()
{
	std::cout << "Serv_config destructor called." << std::endl;
}

Serv_config::Serv_config(const Serv_config &other)
{
	if (this != &other)
	{
		this->_servName = other._servName;
		this->_port = other._port;
		this->_ip = other._ip;
		this->_root = other._root;
		this->_index = other._index;
		this->_error_page = other._error_page;
		this->_timeout = other._timeout;
		this->_listen_fd = other._listen_fd;
		this->_max_body_size = other._max_body_size;
		this->_locations = other._locations;
	}
	std::cout << "Serv_config copy constructor called." << std::endl;
}

Serv_config &Serv_config::operator=(const Serv_config &other)
{
	if (this != &other)
	{
		this->_servName = other._servName;
		this->_port = other._port;
		this->_ip = other._ip;
		this->_timeout = other._timeout;
		this->_listen_fd = other._listen_fd;
		std::cout << "Serv_config assignment operator called." << std::endl;
	}
	return *this;
}

/**************************** Geters and Setters ***********************************/

void Serv_config::setPort(std::string string_port)
{
	int port = std::atoi(string_port.c_str());
	if (port < 0 || port > 65535)
	{
		std::cerr << "Error: Invalid port number. Must be between 0 and 65535." << std::endl;
		return;
	}
	this->_port = port;
}

int Serv_config::getPort() const
{
	return this->_port;
}

void Serv_config::setIp(std::string ip)
{
	if (ip.empty())
	{
		std::cerr << "Error: IP address cannot be empty." << std::endl;
		return;
	}
	if (ip == "localhost")
		this->_ip = inet_addr("127.0.0.1");
	else
		this->_ip = inet_addr(ip.c_str());
}

in_addr_t Serv_config::getIp() const
{
	return this->_ip;
}

void Serv_config::setTimeout(int timeout)
{
	if (timeout < 0)
	{
		std::cerr << "Error: Timeout must be a positive integer." << std::endl;
		return;
	}
	this->_timeout = timeout;
}

int Serv_config::getTimeout() const
{
	return this->_timeout;
}

void Serv_config::setServName(const std::string &servName)
{
	if (servName.empty())
	{
		std::cerr << "Error: Server name cannot be empty." << std::endl;
		return;
	}
	this->_servName = servName;
}

const std::string &Serv_config::getServName() const
{
	return this->_servName;
}

void Serv_config::setListenFd(int listen_fd)
{
	
	this->_listen_fd = listen_fd;
}

int Serv_config::getListenFd() const
{
	return this->_listen_fd;
}

void Serv_config::setRoot(const std::string &root)
{
	if (root.empty())
	{
		std::cerr << "Error: Root directory cannot be empty." << std::endl;
		return;
	}
	this->_root = root;
}

std::string Serv_config::getRoot() const
{
	return this->_root;
}

void Serv_config::setIndex(const std::string &index)
{
	if (index.empty())
	{
		std::cerr << "Error: Index file cannot be empty." << std::endl;
		return;
	}
	this->_index = index;
}

std::string Serv_config::getIndex() const
{
	return this->_index;
}

void Serv_config::setMaxBodySize(std::string size)
{
	if (size.empty())
	{
		std::cerr << "Error: Max body size cannot be empty." << std::endl;
		return;
	}
	try
	{
		this->_max_body_size = _getConvertedMaxSize(size);
	}
	catch (const std::runtime_error &e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
		return;
	}
}

int Serv_config::getMaxBodySize() const
{
	return this->_max_body_size;
}

void Serv_config::setErrorPage(const std::string &error_page)
{
	if (error_page.empty())
	{
		std::cerr << "Error: Error page cannot be empty." << std::endl;
		return;
	}
	this->_error_page = error_page;
}

std::string Serv_config::getErrorPage() const
{
	return this->_error_page;
}

locationMap const &Serv_config::getLocations() const
{
	return this->_locations;
}

void Serv_config::addLocation(const std::string &path, const location_t &location)
{
	if (path.empty())
	{
		std::cerr << "Error: Location path cannot be empty." << std::endl;
		return;
	}
	std::make_pair(path, location);
	this->_locations.insert(std::make_pair(path, location));
}
/**************************** Privates functions ************************************/

//Convert the max body size string in size_t, the string can contain the value in 1G or mb etc..
size_t Serv_config::_getConvertedMaxSize(std::string const &size)
{
	std::string	c;
	std::string	sub;
	double		tmp = 0;
	
	if (!isdigit(size[size.length() - 1]))
	{
		sub = size.substr(0, size.length() - 1);
		c = size.substr(size.length() - 1);
	}
	if ((c.empty() && size.find_first_not_of(DECIMAL) != std::string::npos)
		|| c.find_first_not_of(SUFIX_BYTES) != std::string::npos || sub.find_first_not_of(DECIMAL) != std::string::npos)
		throw std::runtime_error(ERR_MAX_SIZE_INPUT(size));
	if (c.empty())
		tmp = std::atof(size.c_str());
	else if (c == "b" || c == "B")
		tmp = std::atof(sub.c_str());
	else if (c == "k" || c == "K")
		tmp = std::atof(sub.c_str()) * 1024;
	else if (c == "m" || c == "M")
		tmp = std::atof(sub.c_str()) * 1024 * 1024;
	else if (c == "g" || c == "G")
		tmp = std::atof(sub.c_str()) * 1024 * 1024 * 1024;
	if (tmp > MAX_SIZE_LIMIT)
		throw std::runtime_error(ERR_MAX_SIZE_RANGE(size));
	return static_cast<size_t>(tmp);
}

/*********************Output Operator **************************/

//print Server_configue
std::ostream &operator<<(std::ostream &out, Serv_config const &server)
{
	out << "ServerName: " << server.getServName() << std::endl;
	out << "Port: " << server.getPort() << std::endl;
	in_addr ip;
	ip.s_addr = server.getIp();
	out << "Host: " << inet_ntoa(ip) << std::endl;
	out << "Root: " << server.getRoot() << std::endl;
	out << "Index: " << server.getIndex() << std::endl;
	out << "ClientMaxBodySize: " << server.getMaxBodySize() << std::endl;
	out << "ErrorPage: " << server.getErrorPage() << std::endl;
	
	for (locationMap::const_iterator it = server.getLocations().begin(); it != server.getLocations().end(); it++)
	{
		out << "location: " << it->first << std::endl;
		out << "	Root: " << it->second.root << std::endl;
		out << "	Methods: ";

		for (std::vector<std::string>::const_iterator it2 = it->second.methods.begin(); it2 != it->second.methods.end(); it2++)
			out << *it2 << " ";
		out << std::endl;
		out << "	Return: " << it->second.redirect << std::endl;
		out << "	Autoindex: " << it->second.autoindex << std::endl;
		out << "	TryFile: " << it->second.tryFile << std::endl;
		out << "	HasCGI: " << it->second.hasCGI << std::endl;
		out << "	CGIPath: " << it->second.cgiPath << std::endl;
		out << "	CGIExtension: " << it->second.cgiExtension << std::endl;
		out << "	UploadTo: " << it->second.uploadTo << std::endl;
		out << std::endl;
	}
	return out;
}
