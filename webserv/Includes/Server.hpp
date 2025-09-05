/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttreichl <ttreichl@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 16:40:26 by ttreichl          #+#    #+#             */
/*   Updated: 2025/09/05 15:58:55 by ttreichl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "Client.hpp"
#include "Server_configue.hpp"
#include "Response.hpp"
#include <cstdio>
#include <poll.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <cerrno>

class Server
{
	private:
		std::vector<Serv_config> _servs;
		std::vector<Client *> _clients;
		std::vector<pollfd> _poll_fds;
		
		Server();
		
	public:
		Server(std::string configFile);
		~Server();
		Server(const Server &other);
		Server &operator=(const Server &other);
		
		Client* getClient(int fd) const;
		int getIndexPollFd(int fd) const;
		
		void initServer();
		void run();
		void endRunLoop();
		void setSocketNonBlocking(int index);
		void bindSocket(int index);
		void listenSocket(int index);
		void addServerToPollFds();
		bool checkItsServerSocket(int index) const;
		
		void handleClientRead(int fd);
		void handleClientWrite(int fd);
		void acceptNewClient(int index);
		void removeClient(int fd);
		void checkTimeouts();
		void setClientNonBlocking(int fd);
		void addClientToPollFds(int fd, sockaddr_in &client_address, int index);
		
		std::vector<Serv_config> _test_serv_servers();
};

#endif