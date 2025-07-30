/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   recieveRequest.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: proton <proton@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 11:36:32 by proton            #+#    #+#             */
/*   Updated: 2025/07/24 11:30:59 by proton           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "recieveRequest.hpp"

int	beforeRequest( Request& requestInstance, Response& responseInstance, std::string request )
{
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
		//if (requestInstance.getContentLength() < serverInstance.getMaxBodySize())
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
