/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParserCgiResponse.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: proton <proton@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 13:00:37 by bproton           #+#    #+#             */
/*   Updated: 2025/10/02 18:16:03 by proton           ###   ########.fr       */
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
    std::stringstream contentLength;
    std::string status;
    int contentLen;

    if (line[line.length() - 1] == '\r')
    {
        line.erase(line.length() - 1, 1);
    }
	fieldArray = splitField(line, ':');
	if (fieldArray.first.empty() || fieldArray.second.empty())
	{
		requestInstance.setStatusCode(502);
		requestInstance.setErrorBody("Bad Gateway");
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
        contentLength << fieldArray.second;
        if (contentLength.str().empty())
        {
            requestInstance.setStatusCode(502);
            requestInstance.setErrorBody("Bad Gateway");
            return (-1);
        }
        contentLength >> contentLen;
        responseInstance.setContentLength(contentLen);
    }
    else if (fieldArray.first == "Transfer-Encoding")
    {
        if (fieldArray.second != "Chunked")
        {
            requestInstance.setStatusCode(502);
            requestInstance.setErrorBody("Bad Gateway");
            return (-1);
        }
        requestInstance.setChunked(1);
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
    std::string         body;
    
    ss << response;
    
    while (getline(ss, line))
	{
		if (line == "\r" || line.empty())
			break ;
		if (parseHeaders(requestInstance, responseInstance, line ) == -1 )
            return (-1);
	}
    while (getline(ss, line))
        body += line + "\n";
    if (requestInstance.getChunked() == 1)
    {
        
    }
    else
        responseInstance.setBody(body);

    if (requestInstance.getMethode() == "POST")
        responseInstance.setStatusCode(201);
    else
        responseInstance.setStatusCode(200);
    if (responseInstance.getContentLength().empty())
        responseInstance.setContentLength(body.length());
    return (0);
}