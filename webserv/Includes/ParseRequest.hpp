/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParseRequest.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: proton <proton@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 14:54:57 by proton            #+#    #+#             */
/*   Updated: 2025/09/03 14:32:28 by proton           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	PARSEREQUEST_HPP
#define	PARSEREQUEST_HPP

#include "Request.hpp"
#include "Response.hpp"
#include "ParseMultipartFormData.hpp"
#include "ParseWwwFormUrlEncoded.hpp"
#include <iostream>
#include <string>
#include <cstdlib>


int	    parseHttpVersion( std::string httpVersion );
int	    findMarkPoint( std::string uriRequest );
size_t	findBodyStart( std::string request);
int	    countArrayStrings( std::string* array );
int	    searchWhiteSpaceInFieldName( std::string field );
int	    ParseRequestLine( Request& instance, std::string request, Client& clientInstance );
int	    tokeniseRequestField( Request& instance, std::string request );
int	    parseTokenisedHeaderField( Request& instance, Client& clientInstance );
int	    fillContentLength( Request& instance, Response& reponseInstance );
int	    fillContentType( Request& instance, Response& responseInstance );
int	    findChar( std::string str, char c );
int	    fillBody( Request& requestInstance, std::string request, Client& clientInstance );
int	    parseBody( Request& requestInstance, Client& clientInstance, Response& responseInstance );
int	    findInConfigFile( std::string value, std::string key, Client& clientInstance );
int	    isHexadecimal(const std::string &str);


std::string*	splitRequest( std::string request, char separator );
std::string*	splitField( std::string request, char separator );


void	removeIfSpace( std::string* token );


#endif
