/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SendResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: proton <proton@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 11:40:13 by proton            #+#    #+#             */
/*   Updated: 2025/09/10 20:37:26 by proton           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	SENDRESPONSE_HPP
#define	SENDRESPONSE_HPP

#include "Request.hpp"
#include "Response.hpp"
#include "StatusCode.hpp"

int	sendErrorResponse( Request& requestInstance, Response& responseInstance);
void chunkedResponse(Response &responseInstance, Request &requestInstance, Client &clientInstance);
int makeResponse(Request& requestInstance, Response& responseInstance);
std::string genereateHtmlErrorPage(int statusCode, const std::string &errorMessage);

#endif
