/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RecieveRequest.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttreichl <ttreichl@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 11:36:32 by proton            #+#    #+#             */
/*   Updated: 2025/08/11 19:00:00 by ttreichl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RecieveRequest.hpp"
#include "Client.hpp"

int	beforeRequest(Client &ClientInstance)
{
	std::string		request = ClientInstance.getBuffer();
	std::string		line;
	std::stringstream	ssrequest(request);

	Request		requestInstance;
	Response		responseInstance;

	std::getline(ssrequest, line);

	std::cout << "[ Request Line ] : " << line << std::endl;
	if (ParseRequestLine(requestInstance, line) == -1)
	{
		sendErrorResponse(requestInstance, responseInstance);
		return (0);
	}
	std::cout << "[ Request Line Parsed ]" << std::endl;
	while (getline(ssrequest, line))
	{
		if (line == "\r\n" || line == "\n")
			break ;
		if (tokeniseRequestField(requestInstance, line ) == -1 )
		{
			sendErrorResponse(requestInstance, responseInstance);
			return (0);
		}
	}
	std::cout << "[ Request Fields Parsed ]" << std::endl;
	if (parseTokenisedHeaderField(requestInstance) == -1)
	{
		sendErrorResponse(requestInstance, responseInstance);
		return (0);
	}
	std::cout << "[ Tokenised Header Fields Parsed ]" << std::endl;
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
		if (fillBody(requestInstance, request) == -1)
		{
			sendErrorResponse(requestInstance, responseInstance);
			return (0);
		}
		//if (requestInstance.getContentLength() > serverInstance.getMaxBodySize())
		//{
		//	requestInstance.setStatusCode(413);
		//	sendErrorResponse(requestInstance, responseInstance);
		//	return (0);
		//}

	}
	std::cout << "[ Request Body Parsed ]" << std::endl;
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
