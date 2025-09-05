/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParseRequest.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttreichl <ttreichl@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 12:41:17 by proton            #+#    #+#             */
/*   Updated: 2025/09/05 16:16:23 by ttreichl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ParseRequest.hpp"

int	findChar( std::string str, char c )
{
	for (size_t i = 0; i < str.length(); i++)
	{
		if (c == str[i])
			return (1);
	}
	return (0);
}

int	fillContentLength( Request& instance, Response& responseInstance )
{
	std::string	contentLength;
	std::string chunked;
	int			content = 0;

	std::cout << "fill content Length" << std::endl;

	contentLength = instance.getField("Content-Length");
	chunked = instance.getField("Transfer-Encoding");

	if (contentLength.empty() && chunked != "chunked\r")
	{
		instance.setStatusCode(411);
		instance.setErrorBody("Content-Length header is missing");
		return (-1);
	}

	else if (chunked == "chunked\r")
	{
		instance.setChunked(1);
		return (0);
	}
	
	content = atoi(contentLength.c_str());
	if (content < 1)
	{
		instance.setStatusCode(411);
		return (-1);
	}
	instance.setContentLength(atoi(contentLength.c_str()));
	responseInstance.setContentLength(atoi(contentLength.c_str()));

	return (0);
}

int	fillContentType( Request& instance, Response& responseInstance )
{
	std::string	contentType;
	(void)responseInstance;

	std::cout << "in fill content" << std::endl;

	contentType = instance.getField("Content-Type");

	if (contentType.empty())
	{
		instance.setStatusCode(415);
		instance.setErrorBody("Content Type is empty");
		return (-1);

	}
	if (contentType.find("multipart/form-data;boundary=") != std::string::npos)
	{
		instance.setContentType(contentType);
		return (0);
	}
	if (contentType != "application/x-www-form-urlencoded" && contentType != "multipart/form-data")
	{
		instance.setStatusCode(415);
		instance.setErrorBody("Not supported");
		return (-1);
	}
	instance.setContentType(contentType);

	return (0);
}

size_t	findBodyStart( std::string request )
{
	int	i = 0;

	i = request.find("\r\n\r\n");
	if (i != -1)
		return (i);
	i = request.find("\n\n");
	return (i);
}

int	isHexadecimal(const std::string &str)
{
	int value;
	std::stringstream ss(str);
	
	ss << std::hex << str;
	if (ss >> value)
	{
		if (ss.eof())
			return 1;
	}
	return (0);
}

int	setChunkedBody(const std::string &body)
{
	std::string			chunk;
	std::stringstream	ss(body);
	std::string 		line;
	std::string			newBody;

	while(std::getline(ss, line))
	{
		if (line == "0\r\n")
			break ;
		else if (isHexadecimal(line))
			continue ;
		newBody += line;
	}

	return (0);
}

int	fillBody( Request& requestInstance, std::string request, Client& clientInstance )
{
	size_t		start = findBodyStart(request);
	std::string	body;
	(void)clientInstance;

	std::cout << "fill body" << std::endl;

	if (start == std::string::npos)
	{
		requestInstance.setStatusCode(400);
		requestInstance.setErrorBody("Body not found");
		return (-1);
	}

	body = request.substr(start + 4, request.length());
	if (body.empty())
	{
		requestInstance.setStatusCode(400);
		requestInstance.setErrorBody("Body is empty");
		return (-1);
	}

	if (requestInstance.getChunked() == 1)
	{
		setChunkedBody(body);
		return (0);
	}
	
	requestInstance.setBodyStart(body);
	return (0);
}

int	parseBody( Request& requestInstance, Client& clientInstance, Response& responseInstance )
{
	std::string	body = requestInstance.getBodyStart();


	if (requestInstance.getContentType().find("multipart/form-data") != std::string::npos)
	{
		if (parseMultipartFormData(requestInstance, clientInstance, responseInstance) == -1)
			return (-1);
		return (0);
	}

	else if (requestInstance.getContentType() != "x-www-form-urlencoded\r")
	{
		if (parseWwwFormUrlEncoded(requestInstance, body) == -1)
			return (-1);
		responseInstance.setBody("Username created\n");
		responseInstance.setContentType("text/plain");
		responseInstance.setStatusCode(201);
		return (0);
	}

	else if (requestInstance.getContentType() == "image/jpeg\r")
	{
		if (parseJpeg(requestInstance, body) == -1)
			return (-1);
		return (0);
	}
	else
	{
		requestInstance.setStatusCode(415);
		requestInstance.setErrorBody("Unsupported Content-Type");
		return (-1);
	}

	return (0);
}

int	parseHttpVersion( std::string httpVersion )
{
	if (httpVersion != "HTTP/1.1\r" && httpVersion != "HTTP/1.1\n")
		return (-1);
	return (0);
}

int	findMarkPoint( std::string uriRequest )
{
	if (!uriRequest.find('/'))
		return (-1);
	return (0);
}

std::string* splitRequest(std::string request, char separator)
{
	std::string*	tokens = new std::string[3];
	size_t			start = 0;
	size_t			end = 0;
	int				count = 0;

	while (count < 2)
	{
		end = request.find(separator, start);
		if (end == std::string::npos)
		{
			delete[] tokens;
			return NULL;
		}
		tokens[count] = request.substr(start, end - start);
		if (tokens[count].empty())
		{
			delete[] tokens;
			return NULL;
		}
		start = end + 1;
		count++;
	}

	tokens[count] = request.substr(start);
	if (tokens[count].empty())
	{
		delete[] tokens;
		return NULL;
	}

	return tokens;
}

int	countArrayStrings( std::string* array )
{
	int	i = 0;

	while (!array[i].empty())
		i++;
	return (i);
}

int	searchWhiteSpaceInFieldName( std::string field )
{
	int	value = 0;

	value = field.find("	");

	if (value == -1)
		return (0);
	return (-1);
}

void	removeIfSpace( std::string& token )
{
	int	found = 0;

	found = token.find(' ');
	if (found != -1)
		token.erase(found, 1);

	found = token.find_last_of("	");
	if (found == -1)
		return ;
	token.erase(found);
}

std::string* splitField(std::string request, char separator)
{
	std::string*	tokens = new std::string[2];
	size_t			sepPos = request.find(separator);
	size_t			valueStart = sepPos + 1;

	if (sepPos == std::string::npos)
	{
		delete[] tokens;
		return NULL;
	}

	tokens[0] = request.substr(0, sepPos);
	if (findChar(tokens[0], ' ') || findChar(tokens[0], '\t'))
	{
		delete[] tokens;
		return (NULL);
	}

	if (valueStart < request.length() && request[valueStart] == ' ')
		valueStart++;

	tokens[1] = request.substr(valueStart);

	if (tokens[0].empty() || tokens[1].empty())
	{
		delete[] tokens;
		return NULL;
	}

	if (searchWhiteSpaceInFieldName(tokens[0]) == -1)
	{
		delete[] tokens;
		return NULL;
	}

	removeIfSpace(tokens[1]);

	return tokens;
}

void setQuery(std::string uri, Request& instance)
{
	size_t queryPos = uri.find('?');

	if (queryPos != std::string::npos)
	{
		instance.setQuery(uri.substr(queryPos + 1));
		instance.setUri(uri.substr(0, queryPos));
	}
	else
	{
		instance.setQuery("");
	}
}


int ParseRequestLine(Request& instance, std::string request, Client& clientInstance)
{
    std::string uri;
    std::string httpVersion;
    std::string* requestToken;

    if (!request.find("\r\n") && !request.find("\n")) 
	{ 
		instance.setStatusCode(400);
		instance.setErrorBody("Bad Request");
		return (-1);
	}

    requestToken = splitRequest(request, ' ');
    if (requestToken == NULL)
    {
		std::cout << "in token null" << std::endl;
        instance.setStatusCode(400);
        instance.setErrorBody("Bad Request");
        return (-1);
    }

    if (requestToken[0] != "GET" && requestToken[0] != "POST" && requestToken[0] != "DELETE")
    {
        instance.setStatusCode(405);
        instance.setErrorBody("Method Not Allowed");
        delete[] requestToken;
        return (-1);
    }

    if (requestToken[1].find('?') != std::string::npos)
    {
        setQuery(requestToken[1], instance);
        uri = requestToken[1].substr(0, requestToken[1].find('?'));
    }
    else
        uri = requestToken[1];

    std::string root = clientInstance.getServConfig()->getRoot();
	std::string fullPath;
	if (uri == "/")
		fullPath = root + uri + clientInstance.getServConfig()->getIndex();
	else
		fullPath = root + uri;
    if (access(fullPath.c_str(), R_OK) == -1)
    {
        instance.setStatusCode(404);
        instance.setErrorBody("Not Found: " + uri);
        delete[] requestToken;
        return (-1);
    }

    if (requestToken[2].empty())
    {
        instance.setStatusCode(400);
        instance.setErrorBody("Bad Request");
        delete[] requestToken;
        return (-1);
    }
    if (parseHttpVersion(requestToken[2]) == -1)
    {
        instance.setStatusCode(505);
        instance.setErrorBody("HTTP Version Not Supported");
        delete[] requestToken;
        return (-1);
    }

    instance.setMethode(requestToken[0]);
    instance.setUri(fullPath);
    instance.setHttpVersion(requestToken[2]);
    instance.setStatusCode(200);

    delete[] requestToken;
    return (0);
}


int	tokeniseRequestField( Request& instance, std::string request ) // request doit etre seulement les lignes dont j ai besoin
{
	std::string*	fieldArray;

	fieldArray = splitField(request, ':');
	if (fieldArray == NULL)
	{
		std::cout << "filed array null" << std::endl;
		instance.setStatusCode(400);
		instance.setErrorBody("Bad Request in header field");
		return (-1);
	}
	if (fieldArray[0] == "Host" && countArrayStrings( fieldArray ) == 3 )
		instance.setField(fieldArray[0], fieldArray[1] + ':' + fieldArray[2]);
	
	// else if (countArrayStrings( fieldArray ) == 3)
	// {
	// 	instance.setStatusCode( 400 );
	// 	instance.setErrorBody("Bad Request in header field");
	// 	delete[] fieldArray;
	// 	return (-1);
	// }

	else
	{
		if (fieldArray[1].find("\r"))
		{
			std::string::iterator it = fieldArray[1].end();
			--it;
			if (*it == '\r')
				fieldArray[1].erase(it);
		}
		instance.setField(fieldArray[0], fieldArray[1]);
	}

	delete[] fieldArray;

	return (0);
}

int	findInConfigFile(std::string value, std::string key, Client& clientInstance)
{
    if (key == "Host")
    {
        std::string configHost = clientInstance.getServConfig()->getServName();
		std::cout << "configHost: " << configHost << std::endl;
        if (value != configHost && value != configHost + "\r")
            return (-1);
        return (0);
    }
    if (key == "Port")
    {
        int configPort = clientInstance.getServConfig()->getPort();
		std::stringstream ss;
		ss << configPort;
		std::string portStr = ss.str();
        if (value != portStr && value != portStr + "\r")
            return (-1);
        return (0);
    }
    return (-1);
}

int	parseServerNameAndPort(Request& instance, std::string fieldValue, Client& clientInstance)
{
    size_t		colonPos = fieldValue.find(':');
    std::string host;
    std::string port;

    if (colonPos != std::string::npos)
    {
        host = fieldValue.substr(0, colonPos);
        port = fieldValue.substr(colonPos + 1);
    }
    else
    {
        host = fieldValue;
        port = "";
    }

    if (findInConfigFile(host, "Host", clientInstance) == -1)
    {
        instance.setStatusCode(400);
        instance.setErrorBody("Bad Request, host not found in config file");
        return (-1);
    }

    if (!port.empty())
    {
        if (findInConfigFile(port, "Port", clientInstance) == -1)
        {
            instance.setStatusCode(400);
            instance.setErrorBody("Bad Request, port not found in config file");
            return (-1);
        }
    }
    return (0);
}

int	parseConnection( Request& instance, std::string fieldValue )
{
	if (fieldValue.empty())
	{
		instance.setStatusCode(400);
		return (-1);
	}

	if (fieldValue != "keep-alive" && fieldValue != "close")
	{
		instance.setStatusCode(400);
		return (-1);
	}
	return (0);
}

int	parseEachTokens( Request& instance, std::string key, Client& clientInstance )
{
	std::string	assignationKey;

	std::cout << "in parseEachTokens with key: " << key << std::endl;

	assignationKey = instance.getField(key);
	if (assignationKey.empty())
		return (-1);

	if (key == "Host")
	{
		if (parseServerNameAndPort(instance, assignationKey, clientInstance) == -1)
			return (-1);
	}

	else if (key == "Connection")
	{
		if (parseConnection(instance, assignationKey) == -1)
			return (-1);
	}
	return (0);
}

int	parseTokenisedHeaderField( Request& instance, Client& clientInstance )
{
	size_t		fieldLength = instance.getFieldLength();
	size_t		i = 0;
	std::string	key;

	while (i < fieldLength)
	{
		key = instance.getField(i);

		if (parseEachTokens(instance, key, clientInstance) == -1)
			return (-1);
		i++;
	}
	std::cout << "End of parseTokenisedHeaderField" << std::endl;
	return (0);
}
