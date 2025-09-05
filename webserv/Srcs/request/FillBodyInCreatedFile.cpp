/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FillBodyInCreatedFile.cpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: proton <proton@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 13:05:55 by proton            #+#    #+#             */
/*   Updated: 2025/09/02 11:09:54 by proton           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/FillBodyInCreatedFile.hpp"


int fillBodyWwwFormUrlEncoded( Request &requestInstance, Request &tempRequest, std::string& filepath )
{
    if (access(filepath.c_str(), F_OK) == 0)
    {
        requestInstance.setStatusCode(409);
        requestInstance.setErrorBody("File already exists");
        return -1;
    }

    size_t bodySize = tempRequest.getBodyLength();
    std::ofstream newFile(filepath.c_str());
    if (!newFile.is_open())
    {
        requestInstance.setStatusCode(500);
        requestInstance.setErrorBody("Failed to create file");
        return -1;
    }
    for (size_t i = 0; i < bodySize; ++i)
    {
        std::string key = tempRequest.getBody(i, KEY);
        std::string value = tempRequest.getBody(i, VALUE);
        if (key.empty() || value.empty())
            continue;

        newFile << key << " " << value << std::endl;
    }
    newFile.close();
    requestInstance.setStatusCode(201);
    return 0;
}

int fillBodyJpeg( Request &requestInstance, std::string& body, std::string& filepath )
{
    std::ofstream newFile(filepath.c_str(), std::ios::binary);
    if (!newFile.is_open())
    {
        requestInstance.setStatusCode(500);
        requestInstance.setErrorBody("Failed to create file");
        return -1;
    }

    newFile.write(body.c_str(), body.size());
    if (newFile.fail())
    {
        requestInstance.setStatusCode(500);
        requestInstance.setErrorBody("Failed to write to file");
        newFile.close();
        return -1;
    }

    requestInstance.setStatusCode(201);
    newFile.close();
    return 0;
}

int fillBodyTextPlain( Request &requestInstance, std::string& body, std::string& filepath )
{
    std::ofstream newFile(filepath.c_str());
    if (!newFile.is_open())
    {
        requestInstance.setStatusCode(500);
        requestInstance.setErrorBody("Failed to create file");
        return -1;
    }
    newFile << body;
    if (newFile.fail())
    {
        requestInstance.setStatusCode(500);
        requestInstance.setErrorBody("Failed to write to file");
        newFile.close();
        return -1;
    }
    requestInstance.setStatusCode(201);
    newFile.close();
    return 0;
}


