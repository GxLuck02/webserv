/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RecieveCgi.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: proton <proton@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 17:56:14 by proton            #+#    #+#             */
/*   Updated: 2025/09/22 09:36:41 by proton           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RECIEVECGI_HPP
#define RECIEVECGI_HPP

#include "Client.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "StatusCode.hpp"
#include "ParserCgiResponse.hpp"
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <cstring>
#include <sstream>

#define BUFFER_SIZE 1024

int handleCgi(Request &requestInstance, Response &responseInstance, Client &clientInstance);

#endif