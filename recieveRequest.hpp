/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   recieveRequest.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: proton <proton@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 15:04:57 by proton            #+#    #+#             */
/*   Updated: 2025/07/10 10:52:20 by proton           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	RECIEVEREQUEST_HPP
#define RECIEVEREQUEST_HPP

#include "request.hpp"
#include "ParseRequest.hpp"
#include "response.hpp"
#include "sendResponse.hpp"

int	beforeRequest( Request& requestInstance, Response& responseInstance, std::string request );


#endif
