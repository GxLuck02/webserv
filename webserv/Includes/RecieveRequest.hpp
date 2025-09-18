/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RecieveRequest.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: proton <proton@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2025/09/18 17:37:11 by proton           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef	RECIEVEREQUEST_HPP
#define RECIEVEREQUEST_HPP

#include <dirent.h>
#include <sys/stat.h>
#include <dirent.h>
#include <sys/stat.h>
#include "Client.hpp"
#include "Request.hpp"
#include "ParseRequest.hpp"
#include "Response.hpp"
#include "SendResponse.hpp"
#include "Client.hpp"
#include "GetRequest.hpp"
#include "RecieveCgi.hpp"
#include "DeleteRequest.hpp"

int	beforeRequest(Client &ClientInstance, Response &responseInstance);


#endif
