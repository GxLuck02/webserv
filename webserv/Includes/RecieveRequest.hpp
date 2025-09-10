/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RecieveRequest.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: proton <proton@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 15:04:57 by proton            #+#    #+#             */
/*   Updated: 2025/09/10 20:37:12 by proton           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	RECIEVEREQUEST_HPP
#define RECIEVEREQUEST_HPP

#include <dirent.h>
#include <sys/stat.h>
#include "Client.hpp"
#include "Request.hpp"
#include "ParseRequest.hpp"
#include "Response.hpp"
#include "SendResponse.hpp"
#include "Client.hpp"
#include "GetRequest.hpp"
#include "DeleteRequest.hpp"

int	beforeRequest(Client &ClientInstance, Response &responseInstance);


#endif
