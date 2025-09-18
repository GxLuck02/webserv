/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: proton <proton@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 12:22:32 by proton            #+#    #+#             */
/*   Updated: 2025/09/18 17:39:09 by proton           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	REQUEST_HPP
#define	REQUEST_HPP

#include <iostream>
#include <string>
#include <sstream>
#include <unistd.h>
#include <map>
#include <vector>
#include <vector>
#include "Client.hpp"

enum	e_map
{
	KEY = 0,
	VALUE = 1
};

class	Request
{
	private:

		std::string							_methode;
		std::string							_uri;
		std::string							_query;
		std::string							_httpVersion;
		std::string							_location;
		std::string							_location;
		std::map<std::string, std::string>	_field;
		int									_statusCode;
		int									_contentLength;
		int 								_chunked;
		bool								_isFullPath;
		bool								_isFullPath;
		std::string							_contentType;
		std::string							_errorBody;
		std::string							_bodyStart;
		std::map<std::string, std::string>	_body;
		std::vector<std::string>			_autoIndexEntries;
		bool								_isAutoindex;
		bool								_isStaticCgi;

	public:

		Request();
		Request( const Request& copy );
		~Request();

		Request&	operator=( const Request& copy );

		std::string	getMethode() const;
		std::string	getUri() const;
		std::string	getHttpVersion() const;
		std::string	getField( size_t loopRound );
		std::string	getField( std::string key );
		std::string	getContentType() const;
		int			getContentLength() const;
		std::string	getErrorBody() const;
		std::string	getBodyStart() const;
		std::string	getBody( size_t index, enum e_map keyOrValue );
		std::string getQuery() const;
		size_t		getBodyLength() const;
		int			getStatusCode() const;
		int			getFieldLength();
		std::string	getLocation() const;
		std::string	getLocation() const;
		int			getChunked() const;
		void		setMethode( std::string methode );
		void		setUri( std::string uri );
		void		setHttpVersion( std::string httpVersion );
		void		setStatusCode( int status );
		void		setField( std::string key, std::string value );
		void		setContentLength( int length );
		void		setContentType( std::string type );
		void		setErrorBody( std::string body );
		void		setChunked( int chunked );
		void		setBody( std::string key, std::string value );
		void		setBodyStart( std::string bodyStart );
		void		setQuery( std::string query );
		void		setLocation( std::string location );
		void		setAutoindexEntries( std::string &path );
		std::string getAutoIndexEntries( size_t index );
		size_t		getAutoIndexEntriesSize() const;
		void		setIsFullPath( bool trueOrFalse );
		bool		getIsFullPath() const;
		void		setIsAutoIndex( bool autoindex );
		bool		getIsAutoIndex() const;
		void		setIsStaticCgi( bool isStaticCgi );
		bool		getIsStaticCgi() const;

		//std::map<std::string, std::string>::iterator	getFieldIterator( size_t num );
};

#endif
