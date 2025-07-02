/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttreichl <ttreichl@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 16:40:26 by ttreichl          #+#    #+#             */
/*   Updated: 2025/07/02 16:50:23 by ttreichl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "Client.hpp"
#include "Server_configue.hpp"
#include <cstdio>
#include <cerrno>
#include <ctime>

#define MAX_CLIENTS 5 //fd_max

class Server
{
	private:
		// Server configuration
		Serv_config _serv_config;
		int _serv_socket;
		std::vector<Client *> _clients;
		std::vector<pollfd> _poll_fds; //permet de suivre les fds avec poll()
	public:
		Server();
		~Server();
		Server(const Server &other);
		Server &operator=(const Server &other);
		
		Client* getClient(int fd) const;
		int getIndexPollFd(int fd) const;
		void initServer();
		void run();
		void acceptNewClient();
		void handleClientRead(int fd);
		void handleClientWrite(int fd);
		void removeClient(int fd);
		void checkTimeouts();
		
};

#endif