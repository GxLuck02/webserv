/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DeleteRequest.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttreichl <ttreichl@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 12:58:50 by proton            #+#    #+#             */
/*   Updated: 2025/09/09 03:00:19 by ttreichl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DELETEREQUEST_HPP
#define DELETEREQUEST_HPP

#include "Request.hpp"
#include "Response.hpp"
#include "Client.hpp"
#include <fstream>
#include <vector>

int handleDeleteRequest(Request &requestInstance, Response &responseInstance, Client &clientInstance);


#endif