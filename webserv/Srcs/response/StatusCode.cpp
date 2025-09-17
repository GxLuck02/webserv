/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StatusCode.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: proton <proton@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 11:06:27 by proton            #+#    #+#             */
/*   Updated: 2025/09/17 10:13:37 by proton           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/StatusCode.hpp"

std::string	getStatusCodeMessage( int statusCode )
{
	switch ( statusCode )
	{
		case 201:
			return ("Created");

		case 202:
			return ("Accepted");

		case 204:
			return ("No Content");

		case 400:
			return ("Bad Request");

		case 403:
			return ("Forbidden");

		case 404:
			return ("Not Found");

		case 405:
			return ("Method Not Allowed");

		case 409:
			return ("Conflicts");
		
		case 411:
			return ("Length Required");

		case 413:
			return ("Payload Too Large");

		case 415:
			return ("Unsupported Media Type");

		case 500:
			return ("Internal Server Error");

		case 501:
			return ("Not Implemented");
		
		case 505:
			return ("HTTP Version Not Supported");

		default:
			return ("OK");
	}
}
