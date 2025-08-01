/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RecieveRequest.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttreichl <ttreichl@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 11:36:32 by proton            #+#    #+#             */
/*   Updated: 2025/08/01 14:56:35 by ttreichl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RecieveRequest.hpp"
#include "Client.hpp"

int	beforeRequest(Client &ClientInstance)
{
	std::string		request = ClientInstance.getBuffer();
	std::string		line;
	std::stringstream	ssrequest(request);

	std::getline(ssrequest, line);

	if (ParseRequestLine(requestInstance, line) == -1)
	{
		sendErrorResponse(requestInstance, responseInstance);
		return (0);
	}
		
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
	//if (makeResponse(requestInstance, responseInstance) == -1)
	//	return (0);

	return (0);
	
}
