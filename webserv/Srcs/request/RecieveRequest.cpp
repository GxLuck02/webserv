/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RecieveRequest.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: proton <proton@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 11:36:32 by proton            #+#    #+#             */
/*   Updated: 2025/09/07 16:09:46 by proton           ###   ########.fr       */
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

        if (isDirectory(name)) {
            displayName += "/";
        }

        html << "<li><a href=\"" << name << "\">" << displayName << "</a></li>\n";
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

	std::cout << " CURRENT PATH : " << currPath << std::endl;

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

static int findIndexIfNeeded(Request &requestInstance, Client &clientInstance, Response &responseInstance)
{
	if (!isDirectory(requestInstance.getUri()))
		return (0);
	
	std::string requestedLocation = requestInstance.getLocation();
	std::string indexLocation = clientInstance.getServConfig()->getIndexFromLocation(requestedLocation);
	if (indexLocation.empty()) // pas d index dans la location
	{
		std::string	indexServer = clientInstance.getServConfig()->getIndex();
		bool autoindex = clientInstance.getServConfig()->getAutoIndexFromLocation(requestedLocation);
		if (!indexServer.empty()) // index serveur present
		{
			std::string newUri;
			if (requestInstance.getUri() == "/")
				newUri = "/" + indexLocation;
			else
				newUri = requestInstance.getUri() + "/" + indexLocation;
			if (access(newUri.c_str(), F_OK) == -1)
			{
				requestInstance.setStatusCode(403);
				requestInstance.setErrorBody("Forbidden");
				return (-1);
			}
			requestInstance.setUri(newUri);
			return (0);
		}
		if (autoindex == false)
		{
			requestInstance.setStatusCode(403);
			requestInstance.setErrorBody("Forbidden");
			return (-1);
		}
		if (handleAutoIndex(requestInstance, responseInstance, requestInstance.getUri()) == -1)
			return (-1);
	}
	else // si un index est dans la location
	{
		std::string newUri;
		if (requestInstance.getUri() == "/")
			newUri = "/" + indexLocation;
		else
			newUri = requestInstance.getUri() + "/" + indexLocation;
		if (access(newUri.c_str(), F_OK) == -1)
		{
			requestInstance.setStatusCode(403);
			requestInstance.setErrorBody("Forbidden");
			return (-1);
		}
		requestInstance.setUri(newUri);
	}
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

int	beforeRequest(Client &clientInstance)
{
	std::string		request = clientInstance.getBuffer();
	std::string		line;
	std::stringstream	ssrequest(request);
	Request			requestInstance;
	Response		responseInstance;
	
	int				maxBodySize = clientInstance.getServConfig()->getMaxBodySize();

	std::cout << "Received Request:\n" << request << std::endl;
	std::getline(ssrequest, line);

	if (ParseRequestLine(requestInstance, line, clientInstance) == -1)
	{
		std::cout << "Error in ParseRequestLine" << std::endl;
		responseInstance.setStatusCode(requestInstance.getStatusCode());
		sendErrorResponse(requestInstance, responseInstance, clientInstance);
		return (0);
	}
	
	while (getline(ssrequest, line))
	{
		if (line == "\r" || line.empty())
			break ;
		if (tokeniseRequestField(requestInstance, line ) == -1 )
		{
			sendErrorResponse(requestInstance, responseInstance, clientInstance);
			return (0);
		}
	}
	if (requestInstance.getField("Host").empty())
	{
		std::cout << "Host header is missing" << std::endl;
		requestInstance.setStatusCode(400);
		requestInstance.setErrorBody("Bad Request: Host header is missing");
		sendErrorResponse(requestInstance, responseInstance, clientInstance);
		return (0);
	}

	if (parseTokenisedHeaderField(requestInstance, clientInstance) == -1)
	{
		sendErrorResponse(requestInstance, responseInstance, clientInstance);
		return (0);
	}

	if (isMethodAllowed(requestInstance, clientInstance) == -1)
	{
		sendErrorResponse(requestInstance, responseInstance, clientInstance);
		return (0);
	}

	if (requestInstance.getMethode() == "POST")
	{
		std::cout << "In POST method" << std::endl;

		if (fillContentLength(requestInstance, responseInstance) == -1)
		{
			sendErrorResponse(requestInstance, responseInstance, clientInstance);
			return (0);
		}
		if (fillContentType(requestInstance, responseInstance) == -1)
		{
			sendErrorResponse(requestInstance, responseInstance, clientInstance);
			return (0);
		}
		if (requestInstance.getContentLength() > maxBodySize)
		{
			std::cout << "in error maxbody size POST method" << std::endl;
			requestInstance.setStatusCode(413);
			sendErrorResponse(requestInstance, responseInstance, clientInstance);
			return (0);
		}
		if (fillBody(requestInstance, request, clientInstance) == -1)
		{
			sendErrorResponse(requestInstance, responseInstance, clientInstance);
			return (0);
		}
		if (parseBody(requestInstance, clientInstance, responseInstance) == -1)
		{
			sendErrorResponse(requestInstance, responseInstance, clientInstance);
			return (0);
		}
		std::cout << "After post method" << std::endl;
		}

	else if (requestInstance.getMethode() == "GET")
	{
		std::cout << "In GET methode" << std::endl;
		if (findIndexIfNeeded(requestInstance, clientInstance, responseInstance) == -1)
		{
			sendErrorResponse(requestInstance, responseInstance, clientInstance);
			return (0);
		}
		if (handleGetRequest(requestInstance, responseInstance, clientInstance) == -1)
		{
			sendErrorResponse(requestInstance, responseInstance, clientInstance);
			return (0);
		}
	}
	else if (requestInstance.getMethode() == "DELETE")
	{
		std::cout << "In DELETE methode" << std::endl;
		if (handleDeleteRequest(requestInstance, responseInstance, clientInstance) == -1)
		{
			sendErrorResponse(requestInstance, responseInstance, clientInstance);
			return (0);
		}
	}
	else
	{
		requestInstance.setStatusCode(501);
		requestInstance.setErrorBody("Not Implemented: Method not supported");
		sendErrorResponse(requestInstance, responseInstance, clientInstance);
		return (0);
	}

	makeResponse(requestInstance, responseInstance);
	std::cout << "Response to be sent:\n" << responseInstance.getResponse() << std::endl;
	clientInstance.setResponseInstance(responseInstance);
	std::cout << "client response: " << clientInstance.getResponseInstance().getResponse() << std::endl;
	return (1);

}
