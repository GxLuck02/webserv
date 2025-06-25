/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server_configue.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttreichl <ttreichl@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 17:37:46 by ttreichl          #+#    #+#             */
/*   Updated: 2025/06/25 18:00:17 by ttreichl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_CONFIGUE_HPP
#define SERVER_CONFIGUE_HPP

#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <poll.h>
#include <unistd.h>
#include <fcntl.h>
#include <vector>

class Serv_config
{
	private:
		std::string 	_servName;
		int 			_port;
		std::string 	_ip;
		int 			_max_clients;
		int 			_timeout;
	public:
		Serv_config();
		~Serv_config();
		Serv_config(const Serv_config &other);
		Serv_config &operator=(const Serv_config &other);

		const std::string &getServName() const;
		int getPort() const;
		const std::string &getIp() const;
		int getMaxClients() const;
		int getTimeout() const;
};

#endif