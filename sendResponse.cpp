/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sendResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: proton <proton@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 13:24:04 by proton            #+#    #+#             */
/*   Updated: 2025/07/24 16:07:58 by proton           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sendResponse.hpp"

int	sendErrorResponse( Request& requestInstance, Response& responseInstance )
{
	std::stringstream	out;
	std::stringstream	statusMessageLength;
	int			statusCode = requestInstance.getStatusCode();

	out << statusCode;
	statusMessageLength << getStatusCodeMessage(statusCode).length();

	responseInstance.setResponse("HTTP/1.1 " + out.str() + " " + getStatusCodeMessage(statusCode) + "\r\n" +
				"Content-Type: " + "text/plain\r\n" +
				"Content-Length: " + statusMessageLength.str() + "\r\n" + "\r\n" +
				getStatusCodeMessage(statusCode));
				
	return (0);
}

// je dois ajouter l instance du serveur pour que je recuperes les choses du ficher de config
//int	makeResponse( Request& requestInstance, Response& responseInstance )
//{
//	int	statusCode = requestInstance.getStatusCode();
//
//	//responseInstance.setStatusCode(
//}
