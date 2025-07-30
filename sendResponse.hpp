/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sendResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: proton <proton@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 11:40:13 by proton            #+#    #+#             */
/*   Updated: 2025/07/10 14:19:15 by proton           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	SENDRESPONSE_HPP
#define	SENDRESPONSE_HPP

#include "request.hpp"
#include "response.hpp"
#include "StatusCode.hpp"

int	sendErrorResponse( Request& requestInstance, Response& responseInstance );
int	makeResponse( Request& requestInstance, Response& responseInstance );

#endif
