/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GetRequest.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: proton <proton@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 20:25:22 by proton            #+#    #+#             */
/*   Updated: 2025/09/08 02:12:46 by proton           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/GetRequest.hpp"

int handleGetRequest(Request &requestInstance, Response &responseInstance, Client &clientInstance)
{
    std::string uri = requestInstance.getUri();
    std::string fileExtension;
    size_t      extensionFound = uri.find_last_of('.');
    (void)clientInstance;

    if (extensionFound != std::string::npos)
    {
        size_t dotPos = uri.find_first_of('.');
        if (dotPos != std::string::npos)
        {
            size_t extensionEnd = uri.find_first_of("?#", dotPos);
            
            if (extensionEnd != std::string::npos)
                fileExtension = uri.substr(dotPos, extensionEnd - dotPos);
            else
                fileExtension = uri.substr(dotPos);
            
            std::cout << "Extension: " << fileExtension << std::endl;
        }
        else
            fileExtension = uri.substr(extensionFound);
    }

    if (!responseInstance.getBody().empty() && !responseInstance.getContentType().empty())
        return (0);

    else if (!fileExtension.empty())
    {
        
        if (fileExtension == ".html")
        {
            std::ifstream file;
            if (!requestInstance.getQuery().empty())
            {
                size_t queryPos = requestInstance.getUri().find_first_of("?#");
                std::string uriWithoutQuery = requestInstance.getUri().substr(0, queryPos - 0);
                std::cout << uriWithoutQuery << std::endl;
                file.open(uriWithoutQuery.c_str());
            }
            else
                file.open(uri.c_str());
            
            if (!file.is_open())
            {
                requestInstance.setStatusCode(404);
                requestInstance.setErrorBody("Not Found: The requested resource does not exist");
                return -1;
            }
            std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
            responseInstance.setBody(content);
            responseInstance.setContentType("text/html");
            file.close();
        }
        else if (fileExtension == ".jpg" || fileExtension == ".jpeg")
        {
            std::ifstream file(uri.c_str(), std::ios::binary);
            if (!file.is_open())
            {
                requestInstance.setStatusCode(404);
                requestInstance.setErrorBody("Not Found: The requested resource does not exist");
                return -1;
            }
            std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
            responseInstance.setBody(content);
            responseInstance.setContentType("image/jpeg");
            file.close();
        }
        else if (fileExtension == ".ico")
        {
            std::ifstream file(uri.c_str(), std::ios::binary);
            if (!file.is_open())
            {
                requestInstance.setStatusCode(404);
                requestInstance.setErrorBody("Not Found: The requested resource does not exist");
                return -1;
            }
            std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
            responseInstance.setBody(content);
            responseInstance.setContentType("image/x-icon");
            file.close();
        }
        else
        {
            requestInstance.setStatusCode(415);
            requestInstance.setErrorBody("Unsupported Media Type: The requested resource type is not supported");
            return -1;
        }
    }
    std::cout << "GET request handled successfully" << std::endl;

    responseInstance.setStatusCode(200);
    return 0;
}