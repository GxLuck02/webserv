/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RecieveCgi.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: proton <proton@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 11:31:30 by proton            #+#    #+#             */
/*   Updated: 2025/09/18 16:57:56 by proton           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RECIEVECGI_HPP
#define RECIEVECGI_HPP

#include "Client.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "StatusCode.hpp"
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <cstring>
#include <sstream>

#define BUFFER_SIZE 1024

int handleCgi(Request &requestInstance, Response &responseInstance, Client &clientInstance);

#endif