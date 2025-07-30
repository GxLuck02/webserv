/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParseRequest.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: proton <proton@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 14:54:57 by proton            #+#    #+#             */
/*   Updated: 2025/07/23 17:24:08 by proton           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	PARSEREQUEST_HPP
#define	PARSEREQUEST_HPP

#include "request.hpp"
#include "response.hpp"
#include <iostream>
#include <string>
#include <cstdlib>


int	parseHttpVersion( std::string httpVersion );
int	findMarkPoint( std::string uriRequest );
size_t	findBodyStart( std::string request);
int	countWords( std::string request, char separator );
int	countArrayStrings( std::string* array );
int	searchWhiteSpaceInFieldName( std::string field );
int	ParseRequestLine( Request& instance, std::string request );
int	tokeniseRequestField( Request& instance, std::string request );
int	parseTokenisedHeaderField( Request& instance );
int	fillContentLength( Request& instance, Response& reponseInstance );
int	fillContentType( Request& instance, Response& responseInstance );
int	findChar( std::string str, char c );
int	fillBody( Request& requestInstance, std::string request );
int	findInConfigFile( std::string value, std::string key );


std::string*	splitRequest( std::string request, char separator );
std::string*	splitField( std::string request, char separator );


void	removeIfSpace( std::string* token );


#endif
