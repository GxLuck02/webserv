/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RecieveRequest.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: proton <proton@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 11:36:32 by proton            #+#    #+#             */
/*   Updated: 2025/09/01 17:05:16 by proton           ###   ########.fr       */
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

	std::getline(ssrequest, line);

	std::cout << "[ Request Line ] : " << line << std::endl;
	if (ParseRequestLine(requestInstance, line) == -1)
	{
		sendErrorResponse(requestInstance, responseInstance);
		return (0);
	}

	while (getline(ssrequest, line))
	{
		if (line == "\r" || line.empty())
			break ;
		if (tokeniseRequestField(requestInstance, line ) == -1 )
		{
			sendErrorResponse(requestInstance, responseInstance);
			return (0);
		}
	}

	if (parseTokenisedHeaderField(requestInstance) == -1)
	{
		sendErrorResponse(requestInstance, responseInstance);
		return (0);
	}

	if (requestInstance.getMethode() == "POST")
	{
		if (fillContentLength(requestInstance, responseInstance) == -1)
		{
			sendErrorResponse(requestInstance, responseInstance);
			return (0);
		}
		if (fillContentType(requestInstance, responseInstance) == -1)
		{
			sendErrorResponse(requestInstance, responseInstance);
			return (0);
		}
		if (requestInstance.getContentLength() > clientInstance.getMaxBodySize())
		{
			requestInstance.setStatusCode(413);
			sendErrorResponse(requestInstance, responseInstance);
			return (0);
		}
		if (fillBody(requestInstance, request, clientInstance) == -1)
		{
			sendErrorResponse(requestInstance, responseInstance);
			return (0);
		}
		if (parseBody(requestInstance, clientInstance, responseInstance) == -1)
		{
			sendErrorResponse(requestInstance, responseInstance);
			return (0);
		}
		}

	else if (requestInstance.getMethode() == "GET")
	{
		if (handleGetRequest(requestInstance, responseInstance, clientInstance) == -1)
		{
			sendErrorResponse(requestInstance, responseInstance);
			return (0);
		}
	}
	else if (requestInstance.getMethode() == "DELETE")
	{
		if (handleDeleteRequest(requestInstance, responseInstance, clientInstance) == -1)
		{
			sendErrorResponse(requestInstance, responseInstance);
			return (0);
		}
	}
	else
	{
		requestInstance.setStatusCode(501);
		requestInstance.setErrorBody("Not Implemented: Method not supported");
		sendErrorResponse(requestInstance, responseInstance);
		return (0);
	}

	}
	//if (makeResponse(requestInstance, responseInstance) == -1)
	//	return (0);
		std::cout << responseInstance.getResponse() << std::endl;
		std::cout << requestInstance.getMethode() << std::endl;
		std::cout << requestInstance.getUri() << std::endl;
		std::cout << requestInstance.getHttpVersion() << std::endl;
		std::cout << requestInstance.getStatusCode() << std::endl;
		std::cout << requestInstance.getField("Host") << std::endl;
		std::cout << requestInstance.getField("Connection") << std::endl;

	return (0);
	
}
