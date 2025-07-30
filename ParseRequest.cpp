/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParseRequest.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: proton <proton@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 12:41:17 by proton            #+#    #+#             */
/*   Updated: 2025/07/29 11:04:36 by proton           ###   ########.fr       */
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
	int			content = 0;

	contentLength = instance.getField("Content-Length");

	if (contentLength.empty())
	{
		instance.setStatusCode(411);
		return (-1);
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

	contentType = instance.getField("Content-Type");

	if (contentType.empty())
	{
		instance.setStatusCode(415);
		return (-1);

	}
	if (contentType != "x-www-form-urlencoded\r")
	{
		instance.setStatusCode(415);
		return (-1);
	}

	instance.setContentType(contentType);
	responseInstance.setContentType(contentType);

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

int	fillBody( Request& requestInstance, std::string request )
{
	int		start = findBodyStart(request);
	std::string	body;
	size_t		eqPos = 0;
	size_t		ampPos = 0;
	std::string	key;
	std::string	value;

	if (start == -1)
	{
		requestInstance.setStatusCode(400);
		return (-1);
	}

	body = request.substr(start + 4, request.length());
	
	while (start < start + requestInstance.getContentLength())
	{
		eqPos = body.find('=', start);
		if (eqPos == std::string::npos)
			break;

		key = body.substr(start, eqPos - start);
		
		ampPos = body.find('&', eqPos + 1);

		if (ampPos == std::string::npos)
		{
			value = body.substr(eqPos + 1);
			start = body.length();
		}
		else 
		{
			value = body.substr(eqPos + 1, ampPos - eqPos - 1);
			start = ampPos + 1;
		}
		requestInstance.setBody(key, value);
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


int ParseRequestLine( Request& instance, std::string request )
{
	std::string	uri;
	std::string	httpVersion;
	std::string*	requestToken;
	
	if (!request.find("\r\n") && !request.find("\n"))
	{
		instance.setStatusCode(400);
		return (-1);
	}
	requestToken = splitRequest( request, ' ' );
	if (requestToken == NULL)
	{
		instance.setStatusCode(400);
		return (-1);
	}
	
	if (requestToken[0] != "GET" && requestToken[0] != "POST" && requestToken[0] != "DELETE")
	{
		instance.setStatusCode(405);
		return (-1);
	}	
	
	//if (findMarkPoint(requestToken[1]))
	//{
	//	// a faire plus tard
	//	// parseQuery(requestToken[1]);
	//}

	else if (access(requestToken[1].c_str(), R_OK) == -1)
	{
		instance.setStatusCode(404);
		return (-1);
	}
	
	if (requestToken[2].empty())
	{
		instance.setStatusCode(400);
		return (-1);
	}
	if (parseHttpVersion(requestToken[2]) == -1)
	{
		instance.setStatusCode(505);
		return (-1);
	}
	
	instance.setMethode(requestToken[0]);
	instance.setUri(requestToken[1]);
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
		instance.setStatusCode(400);
		return (-1);
	}
	if (fieldArray[0] == "Host" && countArrayStrings( fieldArray ) == 3 )
		instance.setField(fieldArray[0], fieldArray[1] + ':' + fieldArray[2]);
	
	else if (countArrayStrings( fieldArray ) == 3)
	{
		instance.setStatusCode( 400 );
		return (-1);
	}

	else
		instance.setField(fieldArray[0], fieldArray[1]);

	delete[] fieldArray;

	return (0);
}

int	findInConfigFile( std::string value, std::string key )
{
	if (key == "Server-Name")
	{
		return (0);
	}
	if (key == "Port")
	{
		if (value != "8080\r")
			return (-1);
		return (0);
	}
	return (-1);
}

int	parseServerNameAndPort( Request& instance, std::string fieldValue )
{
	if (findInConfigFile(fieldValue, "Server-Name") == -1)
	{
		instance.setStatusCode(400);
		return (-1);
	}

	if (findChar(fieldValue, ':'))
	{
		if (findInConfigFile(fieldValue.substr(fieldValue.find(':') + 1, fieldValue.length()), "Port") == -1)
		{
			instance.setStatusCode(400);
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

	if (fieldValue != "Keep-alive" && fieldValue != "Close")
	{
		instance.setStatusCode(400);
		return (-1);
	}
	return (0);
}

int	parseEachTokens( Request& instance, std::string key )
{
	std::string	assignationKey;

	assignationKey = instance.getField(key);
	if (assignationKey.empty())
		return (-1);

	if (key == "Host")
	{
		std::cout << "IN HOST" << std::endl;
		if (parseServerNameAndPort(instance, assignationKey) == -1)
			return (-1);
	}

	else if (key == "Connection")
	{
		if (parseConnection(instance, assignationKey) == -1)
			return (-1);
	}

	else if (key == "Content-Length" || key == "Content-Type")
		return (0);

	else
	{
		instance.setStatusCode(501);
		return (-1);
	}
	return (0);
}

int	parseTokenisedHeaderField( Request& instance )
{
	size_t		fieldLength = instance.getFieldLength();
	size_t		i = 0;
	std::string	key;

	while (i < fieldLength - 1)
	{
		key = instance.getField(i);

		if (parseEachTokens(instance, key) == -1)
			return (-1);
		i++;
	}
	return (0);
}
