/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SendResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: proton <proton@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 13:24:04 by proton            #+#    #+#             */
/*   Updated: 2025/09/04 17:48:18 by proton           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SendResponse.hpp"

void chunkedResponse(Response &responseInstance, Request &requestInstance, Client &clientInstance)
{
    std::string body = responseInstance.getBody();
    size_t bodyLength = body.length();
    size_t chunkSize = clientInstance.getServConfig()->getMaxBodySize();
    size_t offset = 0;
    int clientSocket = clientInstance.getFd();
    (void)requestInstance; // Unused parameter

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

int	sendErrorResponse(Request& requestInstance, Response& responseInstance, Client& clientInstance)
{
    int statusCode = requestInstance.getStatusCode();
    std::string errorMessage = requestInstance.getErrorBody();
    //size_t maxBodySize = clientInstance.getServConfig()->getMaxBodySize();

    // if (errorMessage.length() > maxBodySize)
    // {
    //     std::cout << "Error message too large, sending chunked response" << std::endl;
    //     chunkedResponse(responseInstance, requestInstance, clientInstance);
    //     return 0;
    // }
    std::cout << errorMessage << std::endl;

    std::stringstream out;
    out << "HTTP/1.1 " << statusCode << " " << getStatusCodeMessage(statusCode) << "\r\n";
    out << "Content-Type: text/plain\r\n";
    out << "Content-Length: " << errorMessage.length() << "\r\n";
    out << "\r\n";
    out << errorMessage;

    responseInstance.setResponse(out.str());
    send(clientInstance.getFd(), responseInstance.getResponse().c_str(), responseInstance.getResponse().length(), 0);

    return 0;
}

int makeResponse(Request& requestInstance, Response& responseInstance, Client& clientInstance)
{
    int statusCode = responseInstance.getStatusCode();
    std::string statusMsg = getStatusCodeMessage(statusCode);
    std::string contentType = responseInstance.getContentType();
    std::string body = responseInstance.getBody();
    (void)requestInstance;

    std::cout << "Response Body: " << body << std::endl;
    std::stringstream out;
    out << "HTTP/1.1 " << statusCode << " " << statusMsg << "\r\n";
    out << "Content-Type: " << contentType << "\r\n";
    out << "Content-Length: " << body.length() << "\r\n";
    out << "\r\n";
    out << body;

    std::cout << "Body is : " << body << std::endl;
    std::cout << "out is : " << out.str() << std::endl;

    responseInstance.setResponse(out.str());
    send(clientInstance.getFd(), responseInstance.getResponse().c_str(), responseInstance.getResponse().length(), 0);
    return 0;
}
