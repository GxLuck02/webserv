/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server_configue.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmontani <tmontani@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 17:37:46 by ttreichl          #+#    #+#             */
/*   Updated: 2025/10/01 14:28:58 by tmontani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_CONFIGUE_HPP
#define SERVER_CONFIGUE_HPP

#include <iostream>
#include <string>
#include <arpa/inet.h>
#include <map>
#include <vector>
#include <cstdlib>

typedef struct location_s
{
	std::string					root;
	std::string					index;
	std::vector<std::string>	methods;
	std::string					redirect;
	bool						autoindex;
	std::string					tryFile;
	bool						hasCGI;
	std::string					cgiPath;
	std::string					cgiExtension;
	std::string					uploadTo;
}				location_t;

typedef std::map<std::string, location_t> locationMap;

class Serv_config
{
	private:
		std::string 	_servName;
		int 			_port;
		in_addr_t		_ip;
		std::string 	_root;
		std::string 	_index;
		std::string 	_error_page;
		int 			_timeout;
		int 			_listen_fd;
		size_t			_max_body_size;
		locationMap		_locations;
		int 			_cgiTimeout;
		
		size_t _getConvertedMaxSize(std::string const &size);
		
	public:
		Serv_config();
		~Serv_config();
		Serv_config(const Serv_config &other);
		Serv_config &operator=(const Serv_config &other);

		//Getters and Setters
		void setServName(const std::string &servName);
		const std::string &getServName() const;
		void setPort(std::string port);
		int getPort() const;
		void setIp(std::string ip);
		in_addr_t getIp() const;
		void setTimeout(int timeout);
		int getTimeout() const;
		void setListenFd(int listen_fd);
		int getListenFd() const;
		void setRoot(const std::string &root);
		std::string getRoot() const;
		void setIndex(const std::string &index);
		std::string getIndex() const;
		void setMaxBodySize(std::string size);
		int getMaxBodySize() const;
		void setErrorPage(const std::string &error_page);
		std::string getErrorPage() const;
		std::string getIndexFromLocation(const std::string &location) const;
		std::string getRootFromLocation(const std::string &location) const;
		bool getAutoIndexFromLocation(const std::string &location) const;
		int checkMethodInLocation(const std::string &location, const std::string &method) const;
		bool isLocationValid(const std::string &location);
		locationMap const &getLocations() const;
		void setCgiTimeout(const std::string &timeoutStr);
		int getCgiTimeout() const;
		
		//Member functions
		void addLocation(const std::string &path, const location_t &location);
};

std::ostream &operator<<(std::ostream &out, Serv_config const &server);

#endif
