/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RecieveRequest.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttreichl <ttreichl@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 11:36:32 by proton            #+#    #+#             */
/*   Updated: 2025/09/05 16:46:22 by ttreichl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/RecieveRequest.hpp"

int	beforeRequest(Client &clientInstance)
{
	std::string		request = clientInstance.getBuffer();
	std::string		line;
	std::stringstream	ssrequest(request);
	Request			requestInstance;
	Response		responseInstance;
	
	int				maxBodySize = clientInstance.getServConfig()->getMaxBodySize();

	std::cout << "Received Request:\n" << request << std::endl;
	std::getline(ssrequest, line);

	if (ParseRequestLine(requestInstance, line, clientInstance) == -1)
	{
		std::cout << "Error in ParseRequestLine" << std::endl;
		sendErrorResponse(requestInstance, responseInstance, clientInstance);
		return (0);
	}
	
	while (getline(ssrequest, line))
	{
		if (line == "\r" || line.empty())
			break ;
		if (tokeniseRequestField(requestInstance, line ) == -1 )
		{
			sendErrorResponse(requestInstance, responseInstance, clientInstance);
			return (0);
		}
	}
	if (requestInstance.getField("Host").empty())
	{
		std::cout << "Host header is missing" << std::endl;
		requestInstance.setStatusCode(400);
		requestInstance.setErrorBody("Bad Request: Host header is missing");
		sendErrorResponse(requestInstance, responseInstance, clientInstance);
		return (0);
	}

	if (parseTokenisedHeaderField(requestInstance, clientInstance) == -1)
	{
		sendErrorResponse(requestInstance, responseInstance, clientInstance);
		return (0);
	}

	if (requestInstance.getMethode() == "POST")
	{
		std::cout << "In POST method" << std::endl;
		if (fillContentLength(requestInstance, responseInstance) == -1)
		{
			sendErrorResponse(requestInstance, responseInstance, clientInstance);
			return (0);
		}
		if (fillContentType(requestInstance, responseInstance) == -1)
		{
			sendErrorResponse(requestInstance, responseInstance, clientInstance);
			return (0);
		}
		if (requestInstance.getContentLength() > maxBodySize)
		{
			std::cout << "in error maxbody size POST method" << std::endl;
			requestInstance.setStatusCode(413);
			sendErrorResponse(requestInstance, responseInstance, clientInstance);
			return (0);
		}
		if (fillBody(requestInstance, request, clientInstance) == -1)
		{
			sendErrorResponse(requestInstance, responseInstance, clientInstance);
			return (0);
		}
		if (parseBody(requestInstance, clientInstance, responseInstance) == -1)
		{
			sendErrorResponse(requestInstance, responseInstance, clientInstance);
			return (0);
		}
		std::cout << "After post method" << std::endl;
		}

	else if (requestInstance.getMethode() == "GET")
	{
		std::cout << "In GET methode" << std::endl;
		if (handleGetRequest(requestInstance, responseInstance, clientInstance) == -1)
		{
			sendErrorResponse(requestInstance, responseInstance, clientInstance);
			return (0);
		}
	}
	else if (requestInstance.getMethode() == "DELETE")
	{
		std::cout << "In DELETE methode" << std::endl;
		if (handleDeleteRequest(requestInstance, responseInstance, clientInstance) == -1)
		{
			sendErrorResponse(requestInstance, responseInstance, clientInstance);
			return (0);
		}
	}
	else
	{
		requestInstance.setStatusCode(501);
		requestInstance.setErrorBody("Not Implemented: Method not supported");
		sendErrorResponse(requestInstance, responseInstance, clientInstance);
		return (0);
	}

	makeResponse(requestInstance, responseInstance);
	std::cout << "Response to be sent:\n" << responseInstance.getResponse() << std::endl;
	clientInstance.setResponseInstance(responseInstance);
	std::cout << "client response: " << clientInstance.getResponseInstance().getResponse() << std::endl;
	return (1);

}
