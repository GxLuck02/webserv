/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttreichl <ttreichl@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 15:09:01 by ttreichl          #+#    #+#             */
/*   Updated: 2025/06/06 16:09:04 by ttreichl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

int main()
{
	int server_socket;
	server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (server_socket < 0)
	{
		std::cerr << "Error creating socket" << std::endl;
		return 1;
	}
	std::cout << "Socket created successfully" << std::endl;
	struct sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(8080);
	server_address.sin_addr.s_addr = INADDR_ANY;
	if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
	{
		std::cerr << "Error binding socket" << std::endl;
		return 1;
	}
	if (listen(server_socket, 5) < 0)
	{
		std::cerr << "Error listening on socket" << std::endl;
		return 1;
	}
	std::cout << "Server is listening on port 8080" << std::endl;
	int client_socket;
	client_socket = accept(server_socket, NULL, NULL);
	if (client_socket < 0)
	{
		std::cerr << "Error accepting connection" << std::endl;
		return 1;
	}
	std::cout << "Client connected" << std::endl;
	while (1)
	{
		ssize_t bytes_received;
		char buffer[BUFFER_SIZE];
		bytes_received = recv(client_socket, buffer, BUFFER_SIZE - 1, 0);
		if (bytes_received < 0)
		{
			std::cerr << "Error receiving data" << std::endl;
			break;
		}
		else if (bytes_received == 0)
		{
			std::cout << "Client disconnected" << std::endl;
			break;
		}
		buffer[bytes_received] = '\0';
		std::cout << "Received: " << buffer << std::endl;

		std::string response = "Echo: " + std::string(buffer);
		size_t bytes_sent = send(client_socket, response.c_str(), response.size(), 0);
	}
	close(client_socket);
	close(server_socket);
	return 0;
}
