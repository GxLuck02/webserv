/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RecieveRequest.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttreichl <ttreichl@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 15:04:57 by proton            #+#    #+#             */
/*   Updated: 2025/08/01 14:57:00 by ttreichl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	RECIEVEREQUEST_HPP
#define RECIEVEREQUEST_HPP

#include "Request.hpp"
#include "ParseRequest.hpp"
#include "Response.hpp"
#include "SendResponse.hpp"

int	beforeRequest(Client &ClientInstance);


#endif
