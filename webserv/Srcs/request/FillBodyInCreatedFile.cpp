/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FillBodyInCreatedFile.cpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: proton <proton@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 13:05:55 by proton            #+#    #+#             */
/*   Updated: 2025/10/08 17:42:44 by proton           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/FillBodyInCreatedFile.hpp"

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


