/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParseMultipartFormData.hpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: proton <proton@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 11:01:50 by proton            #+#    #+#             */
/*   Updated: 2025/09/02 11:05:10 by proton           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSEMULTIPARTFORMDATA_HPP
#define PARSEMULTIPARTFORMDATA_HPP

#include "Request.hpp"
#include "Response.hpp"
#include "FillBodyInCreatedFile.hpp"
#include "ParseWwwFormUrlEncoded.hpp"
#include "ParseJpeg.hpp"
#include <fstream>
#include <vector>

int parseMultipartFormData( Request &requestInstance, Client& client, Response& responseInstance );

#endif