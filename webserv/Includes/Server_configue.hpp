/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server_configue.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttreichl <ttreichl@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 17:37:46 by ttreichl          #+#    #+#             */
/*   Updated: 2025/07/10 19:03:33 by ttreichl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_CONFIGUE_HPP
#define SERVER_CONFIGUE_HPP

#include <iostream>
#include <string>
#include <arpa/inet.h>

class Serv_config
{
	private:
		std::string 	_servName;
		int 			_port;
		in_addr_t		_ip;
		int 			_max_clients;
		int 			_timeout;
		int 			_listen_fd;
		int 			_max_body_size;
		
	public:
		Serv_config();
		~Serv_config();
		Serv_config(const Serv_config &other);
		Serv_config &operator=(const Serv_config &other);

		void setServName(const std::string &servName);
		const std::string &getServName() const;
		void setPort(int port);
		int getPort() const;
		void setIp();
		int getIp() const;
		void setTimeout(int timeout);
		int getTimeout() const;
		void setListenFd(int listen_fd);
		int getListenFd() const;
};

#endif