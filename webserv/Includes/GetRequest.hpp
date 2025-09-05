/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GetRequest.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: proton <proton@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 20:25:42 by proton            #+#    #+#             */
/*   Updated: 2025/08/20 12:56:19 by proton           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GETREQUEST_HPP
#define GETREQUEST_HPP

#include "Request.hpp"
#include "Response.hpp"
#include "Client.hpp"
#include <fstream>
#include <sstream>

int handleGetRequest(Request &requestInstance, Response &responseInstance, Client &clientInstance);

#endif