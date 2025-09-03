/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GetRequest.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: proton <proton@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 20:25:22 by proton            #+#    #+#             */
/*   Updated: 2025/09/02 11:18:29 by proton           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/GetRequest.hpp"

static int searchQueryParameters(const std::string &query, const std::string &uri, Response &responseInstance)
{
    std::ifstream file(uri.c_str());
    (void)query;
    if (!file.is_open())
        return -1;

    std::string pair;
    while (std::getline(file, pair, ':'))
    {
        size_t pos = pair.find('=');
        if (pos == std::string::npos)
            continue;

        std::string key = pair.substr(0, pos);
        std::string value = pair.substr(pos + 1);
        std::string line;
        std::getline(file, line);

        responseInstance.setBody(line);
        responseInstance.setContentType("x-www-form-urlencoded");
        file.close();
        return 0;
    }
    file.close();
    return -1;
}

int handleGetRequest(Request &requestInstance, Response &responseInstance, Client &clientInstance)
{
    std::string uri = requestInstance.getUri();
    std::string fileExtension = uri.substr(uri.find_last_of('.'));
    (void)clientInstance;
    
    if (uri.empty())
    {
        requestInstance.setStatusCode(400);
        requestInstance.setErrorBody("Bad Request: URI is empty");
        return -1;
    }
    else if (access(uri.c_str(), F_OK) == -1)
    {
        requestInstance.setStatusCode(404);
        requestInstance.setErrorBody("Not Found: The requested resource does not exist");
        return -1;
    }

    else if (!requestInstance.getQuery().empty())
    {
        std::string query = requestInstance.getQuery();
        if (searchQueryParameters(query, uri, responseInstance) == -1)
        {
            requestInstance.setStatusCode(400);
            requestInstance.setErrorBody("Bad Request: Invalid query parameters");
            return -1;
        }

    }
    else
    {
        if (fileExtension == ".html")
        {
            std::ifstream file(uri.c_str());
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
        else
        {
            requestInstance.setStatusCode(415);
            requestInstance.setErrorBody("Unsupported Media Type: The requested resource type is not supported");
            return -1;
        }
    }

    responseInstance.setStatusCode(200);
    return 0;
}