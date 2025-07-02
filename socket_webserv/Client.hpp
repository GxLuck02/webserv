/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttreichl <ttreichl@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 16:57:55 by ttreichl          #+#    #+#             */
/*   Updated: 2025/07/02 16:48:41 by ttreichl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <poll.h>
#include <unistd.h>
#include <fcntl.h>
#include <vector>
#include <arpa/inet.h>

class Client
{
	private:
		int 		_fd;
		int			_port;
		in_addr_t	_ip;
		std::string _buffer;
		bool		_isClosed;
		time_t		_lastActivity;
		
	public:
		Client(int fd, int port, in_addr_t ip);
		~Client();
		Client(const Client &other);
		Client &operator=(const Client &other);

		int getFd() const;
		int getPort() const;
		in_addr_t getIp() const;
		time_t getLastActivity() const;
		const std::string &getBuffer() const;
		void appendToBuffer(const std::string& data);
		void clearBuffer();		
		bool isRequestComplete() const;
		bool isClosed() const;
		void setClosed(bool closed);
		void updateLastActivity();
}; 

#endif