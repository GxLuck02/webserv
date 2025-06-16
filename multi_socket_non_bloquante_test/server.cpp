/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttreichl <ttreichl@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 15:09:01 by ttreichl          #+#    #+#             */
/*   Updated: 2025/06/16 16:21:36 by ttreichl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <poll.h>
#include <unistd.h>

#define BUFFER_SIZE 1024
#define MAX_CLIENT	20

int main()
{
	//creation of server socket
	int servSocket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (servSocket_fd < 0)
	{
		std::runtime_error("Error: error in creation of serversocket .");
		return (0);
	}

	//setsockopt for dodge skeep "Address already in use"
	int opt = 1;
  if (setsockopt(servSocket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
			perror("setsockopt");
			close(servSocket_fd);
			return 1;
    }
	
	//set server socket and bind it
	sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(8080);
	bind(servSocket_fd, (struct sockaddr *)&address, sizeof(address));
	if  (bind(servSocket_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
	{
		std::runtime_error("Error: error in bind of serversocket .");
		return (0);
	}
	std::cout << "SOMAXCONN = " << SOMAXCONN << std::endl;
	if (listen(servSocket_fd, SOMAXCONN) < 0)
  {
		std::runtime_error("Error: error in listen of serversocket .");
		return (0);
	}
	

	pollfd	fds[MAX_CLIENT];
	fds[0].;
}
