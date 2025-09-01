/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SendResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: proton <proton@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 13:24:04 by proton            #+#    #+#             */
/*   Updated: 2025/09/01 17:05:43 by proton           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SendResponse.hpp"

void chunkedResponse(Response &responseInstance, Request &requestInstance, Client &clientInstance)
{
    std::string body = responseInstance.getBody();
    size_t bodyLength = body.length();
    size_t chunkSize = clientInstance.getMaxBodySize();
    size_t offset = 0;
    int clientSocket = clientInstance.getSocket();

    while (offset < bodyLength)
    {
        size_t currentChunkSize = std::min(chunkSize, bodyLength - offset);

        std::stringstream chunkHeader;
        chunkHeader << std::hex << currentChunkSize << "\r\n";
        std::string chunk = chunkHeader.str() + body.substr(offset, currentChunkSize) + "\r\n";

        send(clientSocket, chunk.c_str(), chunk.size(), 0);

        offset += currentChunkSize;
    }

    send(clientSocket, "0\r\n\r\n", 5, 0);
}

int	sendErrorResponse( Request& requestInstance, Response& responseInstance, Client& clientInstance )
{
	std::stringstream	out;
	std::stringstream	statusMessageLength;
	int					statusCode = requestInstance.getStatusCode();
	std::string 		errorMessage = requestInstance.getErrorBody();

	out << statusCode;
	statusMessageLength << errorMessage.length();

	if (statusMessageLength > clientInstance.getMaxBodySize())
	{
		chunkedResponse(responseInstance, requestInstance, clientInstance);
		return 0;
	}

	responseInstance.setResponse("HTTP/1.1 " + out.str() + " " + getStatusCodeMessage(statusCode) + "\r\n" +
				"Content-Type: " + "text/plain\r\n" +
				"Content-Length: " + statusMessageLength.str() + "\r\n" + "\r\n" +
				errorMessage);

	return (0);
}

int makeResponse(Request& requestInstance, Response& responseInstance, Client& clientInstance)
{
    int statusCode = responseInstance.getStatusCode();
    std::string statusMsg = getStatusCodeMessage(statusCode);
    std::string contentType = responseInstance.getContentType();
    std::string body = responseInstance.getBody();

    std::stringstream out;
    out << "HTTP/1.1 " << statusCode << " " << statusMsg << "\r\n";
    out << "Content-Type: " << contentType << "\r\n";
    out << "Content-Length: " << body.length() << "\r\n";
    out << "\r\n";
    out << body;

    responseInstance.setResponse(out.str());
    return 0;
}
