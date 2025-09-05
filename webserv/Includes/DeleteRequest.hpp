/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DeleteRequest.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: proton <proton@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 12:58:50 by proton            #+#    #+#             */
/*   Updated: 2025/08/21 12:38:24 by proton           ###   ########.fr       */
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