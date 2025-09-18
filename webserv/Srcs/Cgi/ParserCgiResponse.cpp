/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParserCgiResponse.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: proton <proton@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 13:00:37 by bproton           #+#    #+#             */
/*   Updated: 2025/09/18 17:13:36 by proton           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ParserCgiResponse.hpp"

static void getStatusCode(Response &responseInstance, std::string& status)
{
    std::string         statusCode;
    std::stringstream   ss;
    int                 statusCodeInt;
    
    statusCode = status.substr(0, status.find(' ') -0);
    ss << statusCode;
    ss >> statusCodeInt;

    responseInstance.setStatusCode(statusCodeInt);
}

static int parseHeaders(Request requestInstance, Response &responseInstance, std::string &line)
{
    std::pair<std::string, std::string>	fieldArray;
    std::string contentType;
    std::string contentLength;
    std::string status;

	fieldArray = splitField(line, ':');
	if (fieldArray.first.empty() || fieldArray.second.empty())
	{
		std::cout << "filed array null" << std::endl;
		requestInstance.setStatusCode(400);
		requestInstance.setErrorBody("Bad Request in header field");
		return (-1);
	}
    if (fieldArray.first == "Content-Type")
    {
        contentType = fieldArray.second;
        if (contentType.empty())
        {
            requestInstance.setStatusCode(502);
            requestInstance.setErrorBody("Bad Gateway");
            return (-1);
        }
        responseInstance.setContentType(contentType);
    }
    if (fieldArray.first == "Content-Length")
    {
        contentLength = fieldArray.second;
        if (contentLength.empty())
        {
            requestInstance.setStatusCode(502);
            requestInstance.setErrorBody("Bad Gateway");
            return (-1);
        }
    }
    if (fieldArray.first == "Status")
    {
        status = fieldArray.second;
        if (status.empty())
            return (0);
        getStatusCode(responseInstance, status);
    }
	return (0);
}

int parseResponseCgi(Request &requestInstance, Response &responseInstance, std::string &response)
{
    std::stringstream   ss;
    std::string         line;
    
    ss << response;
    
    while (getline(ss, line))
	{
		if (line == "\r" || line.empty())
			break ;
		if (parseHeaders(requestInstance, responseInstance, line ) == -1 )
            return (-1);
	}
    return (0);
}