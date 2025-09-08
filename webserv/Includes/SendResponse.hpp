/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SendResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttreichl <ttreichl@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 11:40:13 by proton            #+#    #+#             */
/*   Updated: 2025/09/08 19:30:47 by ttreichl         ###   ########.fr       */
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
