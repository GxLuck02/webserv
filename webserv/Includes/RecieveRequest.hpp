/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RecieveRequest.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: proton <proton@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 15:04:57 by proton            #+#    #+#             */
/*   Updated: 2025/09/05 18:12:01 by proton           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	RECIEVEREQUEST_HPP
#define RECIEVEREQUEST_HPP

#include "Client.hpp"
#include "Request.hpp"
#include "ParseRequest.hpp"
#include "Response.hpp"
#include "SendResponse.hpp"
#include "Client.hpp"
#include "GetRequest.hpp"
#include "DeleteRequest.hpp"

int	beforeRequest(Client &ClientInstance);


#endif
