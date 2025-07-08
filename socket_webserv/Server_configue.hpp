/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server_configue.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttreichl <ttreichl@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 17:37:46 by ttreichl          #+#    #+#             */
/*   Updated: 2025/07/02 18:48:51 by ttreichl         ###   ########.fr       */
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
	public:
		Serv_config();
		~Serv_config();
		Serv_config(const Serv_config &other);
		Serv_config &operator=(const Serv_config &other);

		const std::string &getServName() const;
		int getPort() const;
		int getIp() const;
		int getMaxClients() const;
		int getTimeout() const;
};

#endif