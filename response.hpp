/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: proton <proton@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 17:17:26 by proton            #+#    #+#             */
/*   Updated: 2025/07/23 17:15:34 by proton           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	RESPONSE_HPP
#define	RESPONSE_HPP

#include <iostream>
#include <string>
#include <map>

class	Response
{
	private:
		
		std::string				_statusLine;
		std::string				_response;
		std::string				_httpVersion;
		std::map<std::string, std::string>	_header;
		std::string				_contentType;
		std::string				_contentLength;
		int					_statusCode;
		std::map<std::string, std::string>	_body;

	public:

		Response();
		Response( const Response& copy );
		~Response();

		Response&	operator=( const Response& copy );

		std::string	getStatusLine() const;
		int		getStatusCode() const;
		std::string	getHttpVersion() const;
		std::string	getContentType() const;
		std::string	getContentLength() const;
		std::string	getResponse() const;
		void		setResponse( std::string response );
		void		setStatusLine( std::string statusLine );
		void		setStatusCode( int status );
		void		setContentType( std::string type );
		void		setContentLength( int length );
};

#endif
