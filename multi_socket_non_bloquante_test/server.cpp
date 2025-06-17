/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttreichl <ttreichl@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 15:09:01 by ttreichl          #+#    #+#             */
/*   Updated: 2025/06/17 17:12:23 by ttreichl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <poll.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFFER_SIZE 1024
#define MAX_CLIENT	120

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
	int flags = fcntl(servSocket_fd, F_GETFL, 0);
	if (flags < 0)
	{
		perror("fcntl server F_GETFL");
		close(servSocket_fd);
		return 1;
	}
	if (fcntl(servSocket_fd, F_SETFL, flags | O_NONBLOCK) < 0)
	{
		perror("fcntl server F_SETFL");
		close(servSocket_fd);
		return 1;
	}
	//set server socket and bind it
	sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(8080);
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
	//set pollfd for server socket
	// First element is the server socket
	pollfd	fds[MAX_CLIENT];
	fds[0].fd = servSocket_fd;
	fds[0].events = POLLIN;
	fds[0].revents = 0;
	// Initialize the rest of the fds array for client sockets
	//set pollfd for client sockets
	for (int i = 1; i <= MAX_CLIENT; i++)
	{
		fds[i].fd = -1;
		fds[i].events = POLLIN;
		fds[i].revents = 0;
	}
	int client_count = 0;
	while (true)
	{
		int poll_count = poll(fds, MAX_CLIENT + 1, -1);
		if (poll_count < 0)
		{
			std::cerr << "Error in poll" << std::endl;
			break;
		}

		// Check if there is a new connection on the server socket
		if (fds[0].revents & POLLIN)
		{
			int new_socket = accept(servSocket_fd, NULL, NULL);
			if (new_socket < 0)
			{
				std::cerr << "Error accepting new connection" << std::endl;
				continue;
			}
			std::cout << "New client connected: " << new_socket << std::endl;
			int flags = fcntl(new_socket, F_GETFL, 0);
			if (flags < 0)
			{
				perror("fcntl F_GETFL");
				close(new_socket);
				continue;
			}
			if (fcntl(new_socket, F_SETFL, flags | O_NONBLOCK) < 0)
			{
				perror("fcntl F_SETFL");
				close(new_socket);
				continue;
			}
			// Add the new socket to the fds array
			client_count++;
			std::cout << "[0]Current client count: " << client_count << std::endl;
			fds[client_count].fd = new_socket;
			if (client_count > MAX_CLIENT)
				break; // Stop accepting new clients if max reached
		}

		// Check for data on each client socket
		for (int i = 0; i < MAX_CLIENT; i++)
		{
			if (fds[i + 1].fd == -1)
				continue;
			if (fds[i + 1].revents & POLLIN) // +1 because fds[0] is the server socket
			{
				char buffer[BUFFER_SIZE];
				ssize_t bytes_received = recv(fds[i + 1].fd, buffer, BUFFER_SIZE - 1, 0);
				if (bytes_received <= 0)
				{
					std::cout << "Client disconnected: " << fds[i + 1].fd << std::endl;
					client_count--;
					std::cout <<  "[1]Current client count: " << client_count << std::endl;
					close(fds[i + 1].fd);
					fds[i + 1].fd = -1; // Mark as closed
					continue;
				}
				buffer[bytes_received] = '\0';
				std::cout << "Received from client " << fds[i + 1].fd << ": " << buffer << std::endl;

				// Echo back to the client
				send(fds[i + 1].fd, buffer, bytes_received, 0);
			}
			if (fds[i + 1].revents & (POLLERR | POLLHUP))
			{
				std::cerr << "Client disconnected or error on socket: " << fds[i + 1].fd << std::endl;
				close(fds[i + 1].fd);
				client_count--;
				fds[i + 1].fd = -1;
				continue;
			}
		}
	}
	// Close all client sockets
	for (int i = 0; i < MAX_CLIENT; i++)
	{
		if (fds[i].fd == -1)
			continue;
		if (fds[i + 1].fd != -1)
		{
			close(fds[i + 1].fd);
			std::cout << "salut" << std::endl;
			fds[i + 1].fd = -1;
			std::cout << "Closed client socket: " << fds[i + 1].fd << std::endl;
		}
	}
	close(fds[0].fd);
	std::cout << "Server socket closed." << std::endl;
	return 0;
}