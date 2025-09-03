/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParseWwwFormUrlEncoded.cpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: proton <proton@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 11:05:06 by proton            #+#    #+#             */
/*   Updated: 2025/09/02 11:38:15 by proton           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/ParseWwwFormUrlEncoded.hpp"

void cureKeyValue(std::string &key, std::string &value)
{
	if (key.find('%') != std::string::npos)
	{
		size_t pos = 0;
		while ((pos = key.find('%', pos)) != std::string::npos)
		{
			if (pos + 2 < key.size() && isxdigit(key[pos + 1]) && isxdigit(key[pos + 2]))
			{
				std::string hex = key.substr(pos + 1, 2);
				std::stringstream ss(hex);
				char decodedChar;

				ss << std::hex << hex;
				if (ss >> decodedChar)
					value.replace(pos, 3, 1, decodedChar);
			}
			else
				pos++;
		}
	}
	else if (key.find('+') != std::string::npos)
	{
		size_t pos = 0;
		while ((pos = key.find('+', pos)) != std::string::npos)
			key.replace(pos, 1, " ");
	}

	else if (value.find('%') != std::string::npos)
	{
		size_t pos = 0;
		while ((pos = value.find('%', pos)) != std::string::npos)
		{
			if (pos + 2 < value.size() && isxdigit(value[pos + 1]) && isxdigit(value[pos + 2]))
			{
				std::string hex = value.substr(pos + 1, 2);
				std::stringstream ss(hex);
				char decodedChar;

				ss << std::hex << hex;
				if (ss >> decodedChar)
					value.replace(pos, 3, 1, decodedChar);
			}
			else
				pos++;
		}
	}
	else if (value.find('+') != std::string::npos)
	{
		size_t pos = 0;
		while ((pos = value.find('+', pos)) != std::string::npos)
		{
			value.replace(pos, 1, " ");
		}
	}
}

int parseWwwFormUrlEncoded(Request &requestInstance, const std::string &body)
{
	std::string key;
	std::string value;
	size_t      pos = 0;
	size_t      nextPos = 0;

	while ((nextPos = body.find('&', pos)) != std::string::npos)
	{
		std::string pair = body.substr(pos, nextPos - pos);
		size_t eqPos = pair.find('=');
		if (eqPos != std::string::npos)
		{
			key = pair.substr(0, eqPos);
			value = pair.substr(eqPos + 1);
			cureKeyValue(key, value);
			requestInstance.setBody(key, value);
		}
		pos = nextPos + 1;
	}

	if (pos < body.length())
	{
		std::string pair = body.substr(pos);
		size_t eqPos = pair.find('=');
		if (eqPos != std::string::npos)
		{
			key = pair.substr(0, eqPos);
			value = pair.substr(eqPos + 1);
			cureKeyValue(key, value);
			requestInstance.setBody(key, value);
		}
	}

	return 0;
}