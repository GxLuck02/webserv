/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SendResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: proton <proton@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2025/10/02 17:29:35 by proton           ###   ########.fr       */
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
    (void)requestInstance;

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

int	sendErrorResponse(Request& requestInstance, Response& responseInstance)
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
    std::string htmlError = genereateHtmlErrorPage(statusCode, errorMessage);

    std::stringstream out;
    out << "HTTP/1.1 " << statusCode << " " << getStatusCodeMessage(statusCode) << "\r\n";
    out << "Content-Type: text/html\r\n";
    out << "Content-Length: " << htmlError.length() << "\r\n";
    out << "\r\n";
    out << htmlError;

    responseInstance.setResponse(out.str());

    return 0;
}

int makeResponse(Request& requestInstance, Response& responseInstance)
{
    int statusCode = responseInstance.getStatusCode();
    std::string statusMsg = getStatusCodeMessage(statusCode);
    std::string contentType = responseInstance.getContentType();
    std::string body = responseInstance.getBody();
    (void)requestInstance;

    std::stringstream out;
    if (statusCode == 204)
    {
        out << "HTTP/1.1 " << statusCode << " " << statusMsg << "\r\n";
        out << "\r\n";
    }

    else
    {
        out << "HTTP/1.1 " << statusCode << " " << statusMsg << "\r\n";
        out << "Content-Type: " << contentType << "\r\n";
        out << "Content-Length: " << body.length() << "\r\n";
        out << "\r\n";
        out << body;
    }

    responseInstance.setResponse(out.str());
    return 0;
}

std::string genereateHtmlErrorPage(int statusCode, const std::string &errorMessage)
{
    std::stringstream body;
    body << "<!DOCTYPE html>\n"
        << "<html lang=\"fr\">\n"
        << "<head>\n"
        << "  <meta charset=\"UTF-8\" />\n"
        << "  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\" />\n"
        << "  <title>Error " << statusCode << "</title>\n"
        << "  <style>\n"
        << "    body {\n"
        << "      font-family: 'Segoe UI', sans-serif;\n"
        << "      background-image: url('https://pbs.twimg.com/media/GHc9d1PWkAANtXl?format=jpg&name=4096x4096');\n"
        << "      background-size: cover;\n"
        << "      background-position: center;\n"
        << "      background-repeat: no-repeat;\n"
        << "      background-attachment: fixed;\n"
        << "      color: white;\n"
        << "      margin: 0;\n"
        << "      padding: 0;\n"
        << "      height: 100vh;\n"
        << "      position: relative;\n"
        << "    }\n"
        << "    h1 {\n"
        << "      position: absolute;\n"
        << "      top: 20vh;\n"
        << "      left: 50vw;\n"
        << "      transform: translate(-50%, -50%);\n"
        << "      font-size: 4rem;\n"
        << "      font-weight: bolder;\n"
        << "    }\n"
        << "    button {\n"
        << "      position: absolute;\n"
        << "      top: 60vh;\n"
        << "      left: 50vw;\n"
        << "      transform: translate(-50%, -50%);\n"
        << "      width: 300px;\n"
        << "      padding: 9px;\n"
        << "      font-size: 1.2rem;\n"
        << "      background: #c00d07;\n"
        << "      border: none;\n"
        << "      color: rgb(255, 255, 255);\n"
        << "      text-align: center;\n"
        << "      border-radius: 15px;\n"
        << "      cursor: pointer;\n"
        << "      transition: background 0.4s ease;\n"
        << "    }\n"
        << "    button:hover { background: #f17522; }\n"
        << "  </style>\n"
        << "</head>\n"
        << "<body>\n"
        << "  <h1>Error " << statusCode << " " << errorMessage << "</h1>\n"
        << "  <button id=\"retourAccueil\">Retour à l'accueil</button>\n"
        << "  <script>\n"
        << "    const params = new URLSearchParams(window.location.search);\n"
        << "    const error = params.get(\"error\");\n"
        << "    document.getElementById(\"retourAccueil\").onclick = function() {\n"
        << "      window.location.href = \"/index.html\";\n"
        << "    };\n"
        << "    if (error) {\n"
        << "      document.querySelector(\"h1\").textContent = `Error ${error}`;\n"
        << "    }\n"
        << "  </script>\n"
        << "</body>\n"
        << "</html>\n";
    return body.str();
}

