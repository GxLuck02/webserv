/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttreichl <ttreichl@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 15:02:01 by ttreichl          #+#    #+#             */
/*   Updated: 2025/06/06 16:08:52 by ttreichl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

int main()
{
	int socketfd;
	socketfd = socket(AF_INET, SOCK_STREAM, 0);
	if (socketfd < 0)
	{
		std::cerr << "Error creating socket" << std::endl;
		return 1;
	}
	std::cout << "Socket created successfully" << std::endl;
	struct sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(8080);
	inet_pton(AF_INET, "127.0.0.1", &server_address.sin_addr);
	if (connect(socketfd, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
	{
		std::cerr << "Error connecting to server" << std::endl;
		return 1;
	}
	std::cout << "Connected to server" << std::endl;
	while (1)
	{
		std::string message;
		std::cout << "Enter message to send: ";
		std::getline(std::cin, message);
		if (message.empty())
    		continue;
		if (send(socketfd, message.c_str(), message.size(), 0) < 0)
		{
			std::cerr << "Error sending data" << std::endl;
			return 1;
		}
		char buffer[BUFFER_SIZE];
		ssize_t bytes_received = recv(socketfd, buffer, BUFFER_SIZE - 1, 0);
		if (bytes_received < 0)
		{
			std::cerr << "Error receiving data" << std::endl;
			return 1;
		}
		buffer[bytes_received] = '\0';
		std::cout << "Received from server: " << buffer << std::endl;
	}
	close(socketfd);
	return 0;
}

