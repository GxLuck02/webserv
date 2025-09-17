/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParserCgiResponse.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bproton <bproton@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 13:00:53 by bproton           #+#    #+#             */
/*   Updated: 2025/09/17 13:40:59 by bproton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSERCGIRESPONSE_HPP
#define PARSERCGIRESPONSE_HPP

#include "Request.hpp"
#include "Response.hpp"
#include "Client.hpp"
#include "StatusCode.hpp"
#include "ParseRequest.hpp"
#include "SendResponse.hpp"

int parseResponseCgi(Request &requestInstance, Response &responseInstance, Client &clientInstance, std::string &response);

#endif