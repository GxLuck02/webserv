/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttreichl <ttreichl@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 15:31:22 by ttreichl          #+#    #+#             */
/*   Updated: 2025/08/20 15:32:23 by ttreichl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_HPP 
#define PARSER_HPP 
#include <string> 
#include <vector> 
#include <map>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include "Server_configue.hpp"
#include "Define.hpp"


typedef struct LocationConfig
{ 
	std::string path;
	std::map<std::string, std::string> directives;
} LocationConfig;

typedef struct ServerConfig
{ 
	std::map<std::string, std::string> directives;
	std::vector<LocationConfig> locations;
} ServerConfig_t;

class ConfigParser
{
	public:
		ConfigParser(const std::string& path);
		
		std::vector<ServerConfig> getServers() const;
		std::vector<Serv_config> getConfigs() const;
		
		bool checkExtension(const std::string& filename, const std::string& extension) const;
		void fillClassServers();
		bool hasThis(std::string const keyWords, ServerConfig& server);
		void fillServerValues(Serv_config& ParserServ, ServerConfig_t& ParserConfig);
		void fillOptionsValues(Serv_config& ParserServ, ServerConfig_t& ParserConfig);
		void fillLocationValues(Serv_config& ParserServ, ServerConfig_t& ParserConfig);
		bool attributeValue(std::string const keyWords, Serv_config& ServerConfig, ServerConfig_t& ParserConfig);
		bool checkName(std::string name) const;

	private:
		std::vector<ServerConfig> _servers;
		std::vector<Serv_config>  _configs;
		void parseConfigFile(const std::string& path);
		void parseServers(std::istream& stream);
		void parseBlock(std::istream& stream, ServerConfig& server);
};

#endif