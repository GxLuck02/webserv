/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttreichl <ttreichl@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 16:57:55 by ttreichl          #+#    #+#             */
/*   Updated: 2025/06/25 17:57:22 by ttreichl         ###   ########.fr       */
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

class Client
{
	private:
		int 		_fd;
		int			_port;
		int			_ip;
		std::string _buffer;
		bool		_isClosed;
		
	public:
		Client(int fd, int port, int ip);
		~Client();
		Client(const Client &other);
		Client &operator=(const Client &other);

		int getFd() const;
		int getPort() const;
		int getIp() const;
		const std::string &getBuffer() const;
		void setBuffer(const std::string &data);
			
};

#endif