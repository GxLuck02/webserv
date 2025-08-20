/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttreichl <ttreichl@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 12:22:32 by proton            #+#    #+#             */
/*   Updated: 2025/08/01 14:44:44 by ttreichl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	REQUEST_HPP
#define	REQUEST_HPP

#include <iostream>
#include <string>
#include <sstream>
#include <unistd.h>
#include <map>

enum	e_map
{
	KEY = 0,
	VALUE = 1
};

class	Request
{
	private:

		std::string				_methode;
		std::string				_uri;
		std::string				_httpVersion;
		std::map<std::string, std::string>	_field;
		int					_statusCode;
		int					_contentLength;
		std::string				_contentType;
		std::map<std::string, std::string>	_body;

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
		int		getContentLength() const;
		std::string	getBody( std::string key, enum e_map keyOrValue );
		int		getStatusCode() const;
		int		getFieldLength();
		void		setMethode( std::string methode );
		void		setUri( std::string uri );
		void		setHttpVersion( std::string httpVersion );
		void		setStatusCode( int status );
		void		setField( std::string key, std::string value );
		void		setContentLength( int length );
		void		setContentType( std::string type );
		void		setBody( std::string key, std::string value );
		void		eraseMap();

		//std::map<std::string, std::string>::iterator	getFieldIterator( size_t num );
};

#endif
