/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParseRequest.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttreichl <ttreichl@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 12:41:17 by proton            #+#    #+#             */
/*   Updated: 2025/09/28 15:32:00 by ttreichl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ParseRequest.hpp"

/*checks if the uri sent in the request a full path, from root */
// static void identifyRealLocation(std::string &location, Request &requestInstance)
// {
// 	if (location != "/" && (access(location.c_str(), F_OK) != -1))
// 	{
// 		requestInstance.setIsFullPath(true);
// 		return ;
// 	}
// 	requestInstance.setIsFullPath(false);
// }

static void removeFileFromLocation(std::string &location, Request &instance)
{
	std::string newLocation;

	size_t pos = location.find_last_of('/');
	if (pos != std::string::npos)
	{
		newLocation = location.substr(0, pos);
		if (newLocation.empty())
			newLocation = "/";
	}
	instance.setLocation(newLocation);
}

bool isDirectory(const std::string &path)
{
    struct stat s;
    if (stat(path.c_str(), &s) == 0) {
        return S_ISDIR(s.st_mode);
    }
    return false;
}

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
	if (contentType != "application/x-www-form-urlencoded" && contentType != "multipart/form-data" && contentType != "image/jpeg")
	{
		instance.setStatusCode(415);
		instance.setErrorBody("Not supported");
		return (-1);
	}
	instance.setContentType(contentType);
	std::cout << "end of fill content" << std::endl;

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

// Remplace la fonction splitField par :
std::pair<std::string, std::string> splitField(const std::string& request, char separator)
{
    size_t sepPos = request.find(separator);
    size_t valueStart = sepPos + 1;

    if (sepPos == std::string::npos)
        return std::make_pair("", "");

    std::string key = request.substr(0, sepPos);
    if (findChar(key, ' ') || findChar(key, '\t'))
        return std::make_pair("", "");

    if (valueStart < request.length() && request[valueStart] == ' ')
        valueStart++;

    std::string value = request.substr(valueStart);

    if (key.empty() || value.empty())
        return std::make_pair("", "");

    if (searchWhiteSpaceInFieldName(key) == -1)
        return std::make_pair("", "");

    removeIfSpace(value);

    return std::make_pair(key, value);
}

void setQuery(std::string uri, Request& instance)
{
	size_t queryPos = uri.find('?');

	if (queryPos != std::string::npos)
	{
		instance.setQuery(uri.substr(queryPos));
		instance.setUri(uri.substr(0, queryPos));
	}
	else
	{
		instance.setQuery("");
	}
}

static void isExtensionValid(std::string &token, const std::string &cgiExt, Request &requestInstance)
{
	std::string ext;
	size_t pos = token.find_last_of('.');
	if (pos == std::string::npos)
		return ;
	ext = token.substr(pos);
	if (ext != cgiExt)
		requestInstance.setIsStaticCgi(true);
	else
		requestInstance.setIsStaticCgi(false);
}

static int cgiPath(Request &requestInstance, Client &clientInstance, std::string &token)
{
	std::string cgiExt = clientInstance.getServConfig()->getLocations().find("/cgi-bin")->second.cgiExtension;
	std::string cgiPath = clientInstance.getServConfig()->getLocations().find("/cgi-bin")->second.cgiPath;
	std::string root = clientInstance.getServConfig()->getRootFromLocation(requestInstance.getLocation());
	std::string fullPath;

	if (cgiExt.empty())
	{
		requestInstance.setStatusCode(500);
		requestInstance.setErrorBody("Internal Server Error");
		return (-1);
	}
	if (access(cgiPath.c_str(), F_OK) == -1)
	{
		requestInstance.setStatusCode(500);
		requestInstance.setErrorBody("Internal Server Error");
		return (-1);
	}
	if (root.empty())
		root = clientInstance.getServConfig()->getRoot();
	
	isExtensionValid(token, cgiExt, requestInstance);
	if (token.find('?') != std::string::npos)
		fullPath = root + token.substr(0, token.find_first_of('?'));
	else
		fullPath = root + token;
	std::cout << "FULL PATH " << fullPath << std::endl;
	std::cout << "QUETY = " << requestInstance.getQuery() << std::endl;
	if (access(fullPath.c_str(), F_OK) == -1)
	{
		requestInstance.setStatusCode(403);
		requestInstance.setErrorBody("Forbidden");
		return (-1);
	}
	requestInstance.setUri(fullPath);
	return (0);
}

static int	handleFileRequest(Request &requestInstance, Client &clientInstance, std::string &token)
{
	std::string uri;
	std::string root;
	std::string fullPath;
	std::string	testPathQuery;

	//check for fonction with query
	if (looksPercentEncoded(token) == true)
	{
		token = urlDecode(token);
	}
	std::cout << "IS FILE <<<<<<<<<<<<<<<< " << token << std::endl;

	if (token.find('?') != std::string::npos)
    {
        setQuery(token, requestInstance);
        uri = token.substr(0, token.find_first_of('?'));
		std::cout << "QUERY IN GET REQUEST = " << requestInstance.getQuery() << std::endl;
	}
	else
		uri = token;
	removeFileFromLocation(uri, requestInstance);

	root = clientInstance.getServConfig()->getRootFromLocation(uri);

	if (token.find("/cgi-bin") != std::string::npos)
	{
		if (cgiPath(requestInstance, clientInstance, token) == -1)
			return (-1);
		return (0);
	}
	requestInstance.setIsStaticCgi(true);

	if (root.empty())
		root = clientInstance.getServConfig()->getRoot();
	
	fullPath = root + token;
	std::cout << "FULL PATH IN FILE " << fullPath << std::endl;
	if (requestInstance.getQuery().empty())
	{
		if (access(fullPath.c_str(), F_OK) == -1)
		{
			requestInstance.setStatusCode(404);
			requestInstance.setErrorBody("Not found : requested ressource not found\n");
			return (-1);
		}
	}
	else
	{
		testPathQuery = root + "/" + uri;
		if (access(testPathQuery.c_str(), F_OK) == -1)
		{
			requestInstance.setStatusCode(404);
			requestInstance.setErrorBody("Not found : requested ressource not found\n");
			return (-1);
		}
	}
	
	requestInstance.setUri(fullPath);
	return (0);
}

static int handlePostFullPath(Request &requestInstance, Client &clientInstance)
{
	std::string fullPath;
	std::string root;
	std::string location = requestInstance.getLocation();

	root = clientInstance.getServConfig()->getRootFromLocation(location);

	if (root.empty())
		root = clientInstance.getServConfig()->getRoot();
	
	fullPath = root + location;

	if (access(fullPath.c_str(), F_OK) == -1)
	    return (-1);

	requestInstance.setUri(fullPath);
	std::cout << "FULL PATH IN POST" << fullPath << std::endl;
	return (0);
}

static int	handleDirectoryRequest(Request &requestInstance, Client &clientInstance, std::string &token)
{
	std::string uri;
	std::string index;
	std::string	root;
	std::string fullPath;

	uri = token;
	std::cout << "TOKEN IS : " << token << std::endl;

	requestInstance.setLocation(uri);
	requestInstance.setIsStaticCgi(true); // je dois le traiter comme static si c est un dossier

	std::cout << requestInstance.getMethode() << std::endl;
	if (requestInstance.getMethode() == "POST")
	{
		std::cout << "IN POST " << std::endl;
		if (handlePostFullPath(requestInstance, clientInstance) == -1)
		{
			requestInstance.setStatusCode(403);
			requestInstance.setErrorBody("Forbidden");
			return (-1);
		}
		return (0);
	}

	root = clientInstance.getServConfig()->getRootFromLocation(uri);

	if (root.empty())
		root = clientInstance.getServConfig()->getRoot();
	
	if (access((root + uri).c_str(), F_OK) == -1)
	{
		requestInstance.setStatusCode(404);
		requestInstance.setErrorBody("Not found");
		return (-1);
	}

	index = clientInstance.getServConfig()->getIndexFromLocation(uri);
	std::cout << " FIRST CHECK INDEX LOCATION " << index << std::endl;

	if (index.empty())
	{
		index = clientInstance.getServConfig()->getIndex();
		std::cout << "SECOND CHECK INDEX SERVER " << index << std::endl;
		if (index.empty())
		{
			if (clientInstance.getServConfig()->getAutoIndexFromLocation(uri) == false)
			{
				requestInstance.setStatusCode(403);
				requestInstance.setErrorBody("Forbidden");
				return (-1);
			}
			else
			{
				requestInstance.setIsAutoIndex(true);
				fullPath = root + "/" + uri + index;
			}
		}
		else
			fullPath = root + "/" + index;
	}
	else
		fullPath = root + uri + "/" + index;

	std::cout << "FULL PATH " << fullPath << std::endl;

	if (access(fullPath.c_str(), F_OK) == -1)
	{
		requestInstance.setStatusCode(404);
		requestInstance.setErrorBody("Not found : requested ressource not found\n");
		return (-1);
	}
	requestInstance.setUri(fullPath);
	return (0);
}

int	handlePaths(Request &requestInstance, Client& clientInstance, std::string& token)
{
	std::string uri;
	bool		isFile;

	requestInstance.setIsAutoIndex(false);
	if (token.find(".") != std::string::npos)
		isFile = true;
	else
		isFile = false;
	if (isFile == false)
	{
		if (handleDirectoryRequest(requestInstance, clientInstance, token) == -1)
			return (-1);
	}
	else
	{
		if (handleFileRequest(requestInstance, clientInstance, token) == -1)
			return (-1);
	}

	return (0);
}

int ParseRequestLine(Request& instance, std::string request, Client& clientInstance)
{
    std::string uri;
    std::string httpVersion;
    std::string* requestToken;
	std::string fullPath;

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
	instance.setMethode(requestToken[0]);
	if (handlePaths(instance, clientInstance, requestToken[1]) == -1)
	{
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

    instance.setHttpVersion(requestToken[2]);
    instance.setStatusCode(200);

    delete[] requestToken;
    return (0);
}


int	tokeniseRequestField( Request& instance, std::string request ) // request doit etre seulement les lignes dont j ai besoin
{
	std::pair<std::string, std::string>	fieldArray;

	fieldArray = splitField(request, ':');
	if (fieldArray.first.empty() || fieldArray.second.empty())
	{
		std::cout << "filed array null" << std::endl;
		instance.setStatusCode(400);
		instance.setErrorBody("Bad Request in header field");
		return (-1);
	}
	if (fieldArray.first == "Host")
	{
		// If Host header contains a port, it will be in fieldArray.second (e.g. "example.com:8080")
		instance.setField(fieldArray.first, fieldArray.second);
	}
	else
	{
		size_t pos = fieldArray.second.find("\r");
		if (pos != std::string::npos)
		{
			fieldArray.second.erase(pos);
		}
		instance.setField(fieldArray.first, fieldArray.second);
	}

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

    // if (findInConfigFile(host, "Host", clientInstance) == -1)
    // {
    //     instance.setStatusCode(400);
    //     instance.setErrorBody("Bad Request, host not found in config file");
    //     return (-1);
    // }

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

bool looksPercentEncoded(const std::string& s) {
    for (size_t i = 0; i + 2 < s.size(); ++i) {
        if (s[i] == '%' &&
            isxdigit(s[i+1]) &&
            isxdigit(s[i+2])) {
            return true;
        }
    }
    return false;
}

std::string urlDecode(const std::string str) {
    std::string ret;
    char ch;
    int i, ii;
    for (i = 0; i < (int)str.length(); i++) {
        if (int(str[i]) == 37) { // '%'
            sscanf(str.substr(i + 1, 2).c_str(), "%x", &ii);
            ch = static_cast<char>(ii);
            ret += ch;
            i = i + 2;
        } else {
            ret += str[i];
        }
    }
    return ret;
}