#include "Parser.hpp"


ConfigParser::ConfigParser(const std::string& path)
{
	try
	{
		parseConfigFile(path);
		fillClassServers();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
}

/******************************** Getters and Setters ********************************/
std::vector<ServerConfig> ConfigParser::getServers() const
{
	return _servers;
}

std::vector<Serv_config> ConfigParser::getConfigs() const
{
	return _configs;
}

/******************************** Public Methods ********************************/

//check if file extension is same as const extension argument
bool ConfigParser::checkExtension(const std::string& filename, const std::string& extension) const
{
	if (filename.length() < extension.length())
		return false;
	if (filename.substr(filename.length() - extension.length()) == extension)
	{
		std::cout << "File extensiion is : " << filename.substr(filename.length() - extension.length()) << std::endl;
		std::cout << "File extension valid is : " << extension << std::endl;
		return true;
	}
	return false;
}

// This function is intended to fill the ServerConfig class with data from the ConfigParser.
// It can be used to initialize or manipulate server configurations based on parsed data.
void ConfigParser::fillClassServers()
{
	if (_servers.empty())
		throw std::runtime_error("No servers found in the configuration.");
	for (size_t i = 0; i < _servers.size(); ++i)
	{
		Serv_config serverConfig;
		fillServerValues(serverConfig, _servers[i]);
		fillOptionsValues(serverConfig, _servers[i]);
		fillLocationValues(serverConfig, _servers[i]);
		_configs.push_back(serverConfig);
	}
}

//fill the Serve_configue of the server with obligatory values
void ConfigParser::fillServerValues(Serv_config& ServerConfig, ServerConfig_t& ParserConfig)
{
	std::string servName = ParserConfig.directives["server_name"];
	if (!checkName(servName))
		throw std::runtime_error("Server name already exists in the configuration.");
	if (!attributeValue("server_name", ServerConfig, ParserConfig))
		throw std::runtime_error("Server name not specified in the configuration.");
	if (!attributeValue("listen", ServerConfig, ParserConfig))
		throw std::runtime_error("Port not specified in the configuration.");
	if (!attributeValue("host", ServerConfig, ParserConfig))
		throw std::runtime_error("IP address not specified in the configuration.");
	if (!attributeValue("root", ServerConfig, ParserConfig))
		throw std::runtime_error("Root directory not specified in the configuration.");
	if (!attributeValue("client_max_body_size", ServerConfig, ParserConfig))
		throw std::runtime_error("Max body size not specified in the configuration.");
}

//fill the Serve_configue of the server with optrional values
void ConfigParser::fillOptionsValues(Serv_config& ServerConfig, ServerConfig_t& ParserConfig)
{
	if (hasThis("index", ParserConfig))
		ServerConfig.setIndex(ParserConfig.directives["index"]);
	if (hasThis("error_page", ParserConfig))
		ServerConfig.setErrorPage(ParserConfig.directives["error_page"]);
	else
		ServerConfig.setErrorPage("Error.html");
	ServerConfig.setTimeout(TIMEOUT_DEF);
	ServerConfig.setListenFd(-1);
}


void tokenize(const std::string& str, std::vector<std::string>& tokens, char delimiter)
{
	std::stringstream ss(str);
	std::string token;
	while (std::getline(ss, token, delimiter))
	{
		if (!token.empty())
		{
			if (token == "GET" || token == "POST" || token == "DELETE" )
				tokens.push_back(token);
		}
	}
}

//find end fill the location value
void ConfigParser::fillLocationValues(Serv_config& ServerConfig, ServerConfig_t& ParserConfig)
{
	for (size_t i = 0; i <ParserConfig.locations.size(); i++)
	{
		location_t loc;
		LocationConfig ParsLoc = ParserConfig.locations[i];
		if (ParsLoc.directives.find("root") != ParsLoc.directives.end())
			loc.root = ParsLoc.directives["root"];
		else
			loc.root = "";
		loc.autoindex = (ParsLoc.directives["autoindex"] == "on" ? true : false);
		loc.tryFile = ParsLoc.directives["try_file"];
		loc.redirect = ParsLoc.directives["return"];
		loc.cgiPath = ParsLoc.directives["cgi_path"];
		loc.cgiExtension = ParsLoc.directives["cgi_ext"];
		if (!loc.cgiPath.empty() && !loc.cgiExtension.empty())
			loc.hasCGI = true;
		else
			loc.hasCGI = false;
		loc.uploadTo = ParsLoc.directives["upload_to"];
		if (ParsLoc.directives.find("allow_methods") != ParsLoc.directives.end())
			tokenize(ParsLoc.directives["allow_methods"], loc.methods, ' ');
		if (ParsLoc.directives.find("index") != ParsLoc.directives.end())
			loc.index = ParsLoc.directives["index"];
		else
			loc.index = "";
		ServerConfig.addLocation(ParsLoc.path, loc);
	}
}

//function for attribute value from keyWord
bool ConfigParser::attributeValue(std::string const keyWords, Serv_config& ServerConfig, ServerConfig_t& ParserConfig)
{
	if (ParserConfig.directives.find(keyWords) == ParserConfig.directives.end())
	{
		std::cerr << "Error: " << keyWords << " not found in the configuration." << std::endl;
		return false;
	}
	std::string value = ParserConfig.directives[keyWords];
	if (keyWords == "server_name" && hasThis("server_name", ParserConfig))
	{
		ServerConfig.setServName(value);
		return true;
	}
	else if (keyWords == "listen" && hasThis("listen", ParserConfig))
	{
		ServerConfig.setPort(value);
		return true;
	}
	else if (keyWords == "host" && hasThis("host", ParserConfig))
	{
		ServerConfig.setIp(value);
		return true;
	}
	else if (keyWords == "root" && hasThis("root", ParserConfig))
	{
		ServerConfig.setRoot(value);
		return true;
	}
	else if (keyWords == "client_max_body_size" && hasThis("client_max_body_size", ParserConfig))
	{
		ServerConfig.setMaxBodySize(value);
		return true;
	}
	return false;
}

//check if ServerConfig have the value (keyWords)
bool ConfigParser::hasThis(std::string const keyWords, ServerConfig& server)
{
	return (server.directives.find(keyWords) != server.directives.end());
}

//check if the Name of server is already use or not
bool ConfigParser::checkName(std::string servName) const
{
	for (size_t i = 0; i < _configs.size(); i++)
    {
       if (_configs[i].getServName() == servName)
	   {
		   std::cerr << "Error: Server name '" << servName << "' already exists." << std::endl;
		   return false;
	   }
    }
    return true;
}


/******************************** Private Methods ********************************/

void ConfigParser::parseConfigFile(const std::string& path)
{
	if (!checkExtension(path, ".conf"))
	{
		throw std::runtime_error("Invalid file extension. Expected .conf");
	}
	std::ifstream file(path.c_str());
	if (!file.is_open())
	{
		throw std::runtime_error("Cannot open config file: " + path);
	}
	std::string line;
	std::string content;
	while (std::getline(file, line))
	{
		size_t comment = line.find('#');
		if (comment != std::string::npos)
		{
			line = line.substr(0, comment);
		}
		content += line + "\n";
	}
	std::istringstream stream(content);
	parseServers(stream);
}

void ConfigParser::parseServers(std::istream& stream)
{
	std::string word;
	while (stream >> word)
	{
		if (word == "server")
		{
			ServerConfig server;
			parseBlock(stream, server);
			_servers.push_back(server);
		}
	}
}

void ConfigParser::parseBlock(std::istream& stream, ServerConfig& server)
{
	std::string token;
	stream >> token;
	
	if (token != "{")
		throw std::runtime_error("Expected '{' after server");
	while (stream >> token)
	{
		if (token == "}")
			break;
		if (token == "location")
		{
			LocationConfig loc;
			stream >> loc.path;
			stream >> token;
			if (token != "{")
				throw std::runtime_error("Expected '{' after location path");
			while (stream >> token && token != "}")
			{
				std::string value;
				std::string temp;

				while (stream >> temp)
				{
					if (!temp.empty() && temp[temp.length() - 1] == ';')
					{
						temp = temp.substr(0, temp.length() - 1);
						if (!temp.empty())
						{
							if (!value.empty()) value += " ";
								value += temp;
            			}
           				break;
        			}
					else
					{
						if (!value.empty()) value += " ";
						value += temp;
					}
    			}
			loc.directives[token] = value;
			}
			server.locations.push_back(loc);
		}
		else
		{
			std::string value;
			stream >> value;
			if (!value.empty() && value[value.length() - 1] == ';')
				value = value.substr(0, value.length() - 1);
			server.directives[token] = value;
		}
	}
}
