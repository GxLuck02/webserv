/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParseMultipartFormData.cpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: proton <proton@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 11:01:03 by proton            #+#    #+#             */
/*   Updated: 2025/09/16 10:52:56 by proton           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/ParseMultipartFormData.hpp"

std::vector<std::string> split(const std::string &s, const std::string &delimiter)
{
	std::vector<std::string> tokens;
	size_t start = 0, end = 0;

	while ((end = s.find(delimiter, start)) != std::string::npos)
	{
		tokens.push_back(s.substr(start, end - start));
		start = end + delimiter.length();
	}
	tokens.push_back(s.substr(start));
	return tokens;
}

std::string trim(const std::string& s)
{
	const std::string whitespace = " \t\r\n";
	size_t start = s.find_first_not_of(whitespace);
	if (start == std::string::npos)
		return "";
	size_t end = s.find_last_not_of(whitespace);
	return s.substr(start, end - start + 1);
}

// void storeName(const std::string &name, Client& clientInstance)
// {
// 	if (name.empty()) return;

// 	std::string current = clientInstance.getServConfig()->getName();
// 	if (current.empty())
// 		clientInstance.getServConfig()->setName(name);
// 	else
// 		clientInstance.getServConfig()->setName(current + "," + name);
// }

int createFile(Request& requestInstance, std::map<std::string, std::string> &headersMap,\
		std::map<std::string, std::string> &contentDispMap, std::string body)
{
	if (contentDispMap.empty())
		return -1;

	std::string contentType = headersMap["Content-Type"];
	std::string filePath = requestInstance.getUri() + "/" + contentDispMap["filename"];
	
	std::cout << "CONTENT TYPE IS : " << contentType << std::endl;

	if (access(filePath.c_str(), F_OK) == 0)
	{
		requestInstance.setStatusCode(409);
		requestInstance.setErrorBody("File already exists: " + contentDispMap["filename"] + "\n");
		return (-1);
	}

	if (contentType.empty())
	{
		requestInstance.setStatusCode(400);
		requestInstance.setErrorBody("Content-Type header is missing\n");
		return (-1);
	}
	if (contentType == "x-www-form-urlencoded")
	{
		Request tempRequest;
		if (parseWwwFormUrlEncoded(tempRequest, body) == -1)
			return (-1);
		if (fillBodyWwwFormUrlEncoded(requestInstance, tempRequest, filePath) == -1)
			return (-1);
	}
	else if (contentType == "image/jpeg")
	{
		if (parseJpeg(requestInstance, body) == -1)
			return (-1);
		if (fillBodyJpeg(requestInstance, body, filePath) == -1)
			return (-1);
	}
	else if (contentType == "text/plain")
	{
		if (fillBodyTextPlain(requestInstance, body, filePath) == -1)
			return (-1);
	}
	else
	{
		requestInstance.setStatusCode(415);
		requestInstance.setErrorBody("Unsupported Content-Type: " + contentType + "\n");
		return (-1);
	}

	return 0;
}

int parseContentDisposition(Request& requestInstance, const std::string &line,\
		std::map<std::string, std::string> &contentDispMap)
{
	std::istringstream ss(line);
	std::string token;
	(void)requestInstance;

	while (std::getline(ss, token, ';'))
	{
		token = trim(token);
		size_t eq_pos = token.find('=');
		if (eq_pos == std::string::npos)
			continue ;
		// if (ss.str() != "form-data")
		// {
		// 	std::cout << ss.str() << std::endl;
		// 	requestInstance.setStatusCode(400);
		// 	requestInstance.setErrorBody("Invalid Content-Disposition format");
		// 	return (-1);
		// }

		std::string key = trim(token.substr(0, eq_pos));
		std::string value = trim(token.substr(eq_pos + 1));

		if (!value.empty() && value[0] == '"' && value[value.size() - 1] == '"')
			value = value.substr(1, value.size() - 2);

		contentDispMap[key] = value;
	}
	return 0;
}

int parseEachPart(Request &requestInstance, const std::string &part, Client& clientInstance)
{
	size_t pos = part.find("\r\n\r\n");
	(void)clientInstance;
	if (pos == std::string::npos)
	{
		requestInstance.setStatusCode(400);
		requestInstance.setErrorBody("Invalid multipart part format\n");
		return -1;
	}

	std::string headerStr = part.substr(0, pos);
	std::string body = part.substr(pos + 4);
	std::istringstream headerStream(headerStr);
	std::string line;
	std::map<std::string, std::string> headersMap;
	std::map<std::string, std::string> contentDispMap;
	std::map<std::string, std::string>::iterator it;

	while (std::getline(headerStream, line))
	{
		line = trim(line);
		if (line.empty())
			continue ;

		size_t sep = line.find(':');
		if (sep == std::string::npos)
			continue ;

		std::string key = trim(line.substr(0, sep));
		std::string value = trim(line.substr(sep + 1));

		headersMap[key] = value;
	}

	it = headersMap.find("Content-Disposition");
	if (it != headersMap.end())
	{
		if (parseContentDisposition(requestInstance, it->second, contentDispMap) == -1)
			return -1;

		it = contentDispMap.find("filename");

		if (createFile(requestInstance, headersMap, contentDispMap, body) == -1)
			return (-1);
	}
	// else if ((it = headersMap.find("name")) != headersMap.end())
	// {
	// 	if (it != headersMap.end())
	// 		storeName(it->second, clientInstance);
	// }

	return 0;
}

int parseMultipartFormData( Request &requestInstance, Client& clientInstance, Response& responseInstance )
{
	//int maxBodySize = clientInstance.getServConfig()->getMaxBodySize();
	std::string body = requestInstance.getBodyStart();
	std::string contentType = requestInstance.getField("Content-Type");
	size_t bpos = contentType.find("boundary=");

	if (bpos == std::string::npos)
	{
		requestInstance.setStatusCode(400);
		requestInstance.setErrorBody("No boundary found\n");
		return -1;
	}

	std::string boundary = "--" + contentType.substr(bpos + 9);

	while (!body.empty())
	{
		size_t start = body.find(boundary);
		if (start == std::string::npos)
			break ;
		start += boundary.size();

		if (body.substr(start, 2) == "--")
			break ;

		size_t end = body.find(boundary, start);
		if (end == std::string::npos)
			break ;

		std::string part = body.substr(start, end - start);
		if (parseEachPart(requestInstance, part, clientInstance) == -1)
			return -1;

		body = body.substr(end);
	}

	responseInstance.setStatusCode(201);
	responseInstance.setBody("upload successful\n");
	responseInstance.setContentType("text/plain");

	return 0;
}
