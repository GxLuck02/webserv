/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SendResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttreichl <ttreichl@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 11:40:13 by proton            #+#    #+#             */
/*   Updated: 2025/10/08 15:45:41 by ttreichl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	SENDRESPONSE_HPP
#define	SENDRESPONSE_HPP

#include "Request.hpp"
#include "Response.hpp"
#include "StatusCode.hpp"
#include <fstream>

int	sendErrorResponse(Request& requestInstance, Response& responseInstance, Client& clientInstance);
void chunkedResponse(Response &responseInstance, Request &requestInstance, Client &clientInstance);
int makeResponse(Request& requestInstance, Response& responseInstance);
std::string genereateHtmlErrorPageDefault(int statusCode, const std::string &errorMessage);
std::string genereateHtmlErrorPage(std::string filename);

#endif
