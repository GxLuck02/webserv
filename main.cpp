/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: proton <proton@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 10:45:17 by proton            #+#    #+#             */
/*   Updated: 2025/07/28 13:28:21 by proton           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <string>
#include "recieveRequest.hpp"

int main( void )
{
	std::string	teststr("GET /usrbin HTTP/1.1\r\nHost: 127.0.0.0:8080\r\nConnection: Close");
	Request		requestInstance;
	Response	responseInstance;
	std::map<std::string, std::string>::iterator it;

	if (beforeRequest( requestInstance, responseInstance, teststr ) == 0)
	{
		std::cout << responseInstance.getResponse() << std::endl;
		//std::cout << requestInstance.getMethode() << std::endl;
		//std::cout << requestInstance.getUri() << std::endl;
		//std::cout << requestInstance.getHttpVersion() << std::endl;
		//std::cout << requestInstance.getStatusCode() << std::endl;
		//std::cout << requestInstance.getField("Host") << std::endl;
		//std::cout << requestInstance.getField("Connection") << std::endl;
	}
	return (0);
		
}
