/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DeleteRequest.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: proton <proton@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 13:00:02 by proton            #+#    #+#             */
/*   Updated: 2025/08/21 12:38:35 by proton           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/DeleteRequest.hpp"

static int deleteWithQuery(Request &requestInstance, Response &responseInstance, Client &clientInstance)
{
    std::string uri = requestInstance.getUri();
    std::string query = requestInstance.getQuery();
    std::string line;
    bool found = false;
    std::vector<std::string> lines;

    if (query.empty())
    {
        requestInstance.setStatusCode(400);
        requestInstance.setErrorBody("Bad Request: Query parameters are missing");
        return -1;
    }

    std::ifstream file(uri.c_str());
    if (!file.is_open())
    {
        requestInstance.setStatusCode(404);
        requestInstance.setErrorBody("Not Found: The requested resource does not exist");
        return -1;
    }

    while (std::getline(file, line))
    {
        if (line.find(query) != std::string::npos)
        {
            found = true;
            continue;
        }
        lines.push_back(line);
    }
    file.close();

    if (!found)
    {
        requestInstance.setStatusCode(404);
        requestInstance.setErrorBody("Not Found: Query not found in file");
        return -1;
    }

    std::ofstream outfile(uri.c_str(), std::ios::trunc);
    if (!outfile.is_open())
    {
        requestInstance.setStatusCode(500);
        requestInstance.setErrorBody("Internal Server Error: Unable to update the resource");
        return -1;
    }
    for (size_t i = 0; i < lines.size(); ++i)
    {
        outfile << lines[i];
        if (i != lines.size() - 1)
            outfile << "\n";
    }
    outfile.close();

    responseInstance.setStatusCode(204);
    return 0;
}

int handleDeleteRequest(Request &requestInstance, Response &responseInstance, Client &clientInstance)
{
    std::string uri = requestInstance.getUri();

    if (uri.empty())
    {
        requestInstance.setStatusCode(400);
        requestInstance.setErrorBody("Bad Request: URI is empty");
        return -1;
    }

    if (access(uri.c_str(), F_OK) == -1)
    {
        requestInstance.setStatusCode(404);
        requestInstance.setErrorBody("Not Found: The requested resource does not exist");
        return -1;
    }

    if (!requestInstance.getQuery().empty())
    {
        if (deleteWithQuery(requestInstance, responseInstance, clientInstance) == -1)
        {
            requestInstance.setStatusCode(404);
            requestInstance.setErrorBody("Not Found: The requested resource does not exist");
            return (-1);
        }
    }

    else if (remove(uri.c_str()) != 0)
    {
        requestInstance.setStatusCode(500);
        requestInstance.setErrorBody("Internal Server Error: Unable to delete the resource");
        return -1;
    }

    responseInstance.setStatusCode(204);
    return 0;
}