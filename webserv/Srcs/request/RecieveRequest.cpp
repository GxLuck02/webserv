/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RecieveRequest.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: proton <proton@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2025/09/19 16:49:44 by proton           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../Includes/RecieveRequest.hpp"

static int	isParentOrChildDir(std::string name)
{
	if (name == "." || name == "..")
		return (1);
	return (0);
}

void generateAutoIndex(const std::string &uri, Request &requestInstance, Response &responseInstance)
{
    std::ostringstream	html;

	std::cout << " URI ========================= " << uri << std::endl;

    html << "<!DOCTYPE html>\n<html>\n<head>\n";
    html << "<meta charset=\"UTF-8\">\n";
    html << "<title>Index of " << uri << "</title>\n";
    html << "</head>\n<body>\n";
    html << "<h1>Index of " << uri << "</h1>\n";
    html << "<ul>\n";

    for (size_t i = 0; i < requestInstance.getAutoIndexEntriesSize(); ++i)
	{
        std::string name = requestInstance.getAutoIndexEntries(i);
        std::string displayName = name;

        size_t pos = name.find_last_of('/');
        if (pos != std::string::npos)
            displayName = name.substr(pos + 1);
		std::cout << "NAME ====== " << name << std::endl; 
        if (isDirectory(name))
		{
            displayName += "/";
        }

        html << "<li><a href=\"" << displayName << "\">" << displayName << "</a></li>\n";
    }

    html << "</ul>\n</body>\n</html>\n";
    responseInstance.setBody(html.str());
	responseInstance.setStatusCode(200);
	responseInstance.setContentType("text/html");
}


static int handleAutoIndex(Request &requestInstance, Response &responseInstance, std::string currPath)
{
	struct dirent	*currFile;
	DIR*	directory =	opendir(currPath.c_str());

	if (!directory)
	{
		requestInstance.setStatusCode(404);
		requestInstance.setErrorBody("Not Found");
		return -1;
	}

	while ((currFile = readdir(directory)) != NULL)
	{
		if (isParentOrChildDir(std::string(currFile->d_name)))
			continue ;
		
		std::string fileName = currFile->d_name;
		std::string	fullPath = currPath;

		if (fullPath[fullPath.length() - 1] != '/')
			fullPath += "/";
		fullPath += fileName;

		requestInstance.setAutoindexEntries(fullPath);
	}
	closedir(directory);
	generateAutoIndex(requestInstance.getLocation(), requestInstance, responseInstance);
	return (0);
}

static int isMethodAllowed(Request &requestInstance, Client &clientInstance)
{
	std::string method = requestInstance.getMethode();
	std::string location = requestInstance.getLocation();
	int foundMethode = clientInstance.getServConfig()->checkMethodInLocation(location, method);

	if (foundMethode == 0)
	{
		requestInstance.setStatusCode(405);
		requestInstance.setErrorBody("Method Not Allowed: The method is not allowed for the requested location");
		return -1;
	}

	return 0;
}

int	beforeRequest(Client &clientInstance, Response &responseInstance)
{
	std::string		request = clientInstance.getBuffer();
	std::string		line;
	std::stringstream	ssrequest(request);
	Request			requestInstance;
	
	
	int				maxBodySize = clientInstance.getServConfig()->getMaxBodySize();

	std::cout << "Received Request:\n" << request << std::endl;
	std::getline(ssrequest, line);

	if (ParseRequestLine(requestInstance, line, clientInstance) == -1)
	{
		std::cout << "Error in ParseRequestLine" << std::endl;
		responseInstance.setStatusCode(requestInstance.getStatusCode());
		sendErrorResponse(requestInstance, responseInstance);
		return (0);
	}
	
	while (getline(ssrequest, line))
	{
		if (line == "\r" || line.empty())
			break ;
		if (tokeniseRequestField(requestInstance, line ) == -1 )
		{
			sendErrorResponse(requestInstance, responseInstance);
			return (0);
		}
	}
	if (requestInstance.getField("Host").empty())
	{
		requestInstance.setStatusCode(400);
		requestInstance.setErrorBody("Bad Request: Host header is missing");
		sendErrorResponse(requestInstance, responseInstance);
		return (0);
	}

	if (parseTokenisedHeaderField(requestInstance, clientInstance) == -1)
	{
		sendErrorResponse(requestInstance, responseInstance);
		return (0);
	}
	std::cout << "BEFORE METHODE ALLOWED" << std::endl;

	if (isMethodAllowed(requestInstance, clientInstance) == -1)
	{
		sendErrorResponse(requestInstance, responseInstance);
		return (0);
	}

	if (requestInstance.getIsStaticCgi() == false)
	{
		if (handleCgi(requestInstance, responseInstance, clientInstance) == -1)
		{
			sendErrorResponse(requestInstance, responseInstance);
			return (0);
		}
		else
		{
			makeResponse(requestInstance, responseInstance);
			clientInstance.setResponseInstance(responseInstance);
			return (0);
		}
	}

	if (requestInstance.getMethode() == "POST")
	{
		std::cout << "In POST method" << std::endl;

		if (fillContentLength(requestInstance, responseInstance) == -1)
		{
			sendErrorResponse(requestInstance, responseInstance);
			return (0);
		}
		std::cout << "after fill content length" << std::endl;
		if (fillContentType(requestInstance, responseInstance) == -1)
		{
			sendErrorResponse(requestInstance, responseInstance);
			return (0);
		}
		std::cout << "after fill content type" << std::endl;
		if (requestInstance.getContentLength() > maxBodySize)
		{
			std::cout << "in error maxbody size POST method" << std::endl;
			requestInstance.setStatusCode(413);
			sendErrorResponse(requestInstance, responseInstance);
			return (0);
		}
		std::cout << "after max body size" << std::endl;
		if (fillBody(requestInstance, request, clientInstance) == -1)
		{
			sendErrorResponse(requestInstance, responseInstance);
			return (0);
		}
		std::cout << "after fille body" << std::endl;
		if (parseBody(requestInstance, clientInstance, responseInstance) == -1)
		{
			sendErrorResponse(requestInstance, responseInstance);
			return (0);
		}
		std::cout << "After post method" << std::endl;
		}

	else if (requestInstance.getMethode() == "GET")
	{
		std::cout << "In GET methode" << std::endl;
		if (requestInstance.getIsAutoIndex() == true)
		{
			if (handleAutoIndex(requestInstance, responseInstance, requestInstance.getUri()) == -1)
			{
				sendErrorResponse(requestInstance, responseInstance);
				return (0);
			}
		}
		if (handleGetRequest(requestInstance, responseInstance, clientInstance) == -1)
		{
			sendErrorResponse(requestInstance, responseInstance);
			return (0);
		}
	}
	else if (requestInstance.getMethode() == "DELETE")
	{
		std::cout << "In DELETE methode" << std::endl;
		if (handleDeleteRequest(requestInstance, responseInstance, clientInstance) == -1)
		{
			sendErrorResponse(requestInstance, responseInstance);
			return (0);
		}
	}
	else
	{
		requestInstance.setStatusCode(501);
		requestInstance.setErrorBody("Not Implemented: Method not supported");
		sendErrorResponse(requestInstance, responseInstance);
		return (0);
	}

	makeResponse(requestInstance, responseInstance);
	clientInstance.setResponseInstance(responseInstance);
	std::cout << "client response: " << clientInstance.getResponseInstance().getResponse() << std::endl;
	return (1);

}
