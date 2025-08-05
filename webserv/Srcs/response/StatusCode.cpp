/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StatusCode.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: proton <proton@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 11:06:27 by proton            #+#    #+#             */
/*   Updated: 2025/07/28 14:02:37 by proton           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "StatusCode.hpp"

std::string	getStatusCodeMessage( int statusCode )
{
	switch ( statusCode )
	{
		case 201:
			return ("Created");

		case 400:
			return ("Bad Request");

		case 404:
			return ("Not Found");

		case 405:
			return ("Method Not Allowed");
		
		case 411:
			return ("Length Required");

		case 413:
			return ("Payload Too Large");

		case 415:
			return ("Unsupported Media Type");

		case 501:
			return ("Not Implemented");

		default:
			return ("OK");
	}
}
