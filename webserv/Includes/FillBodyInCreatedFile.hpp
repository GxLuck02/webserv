/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FillBodyInCreatedFile.hpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: proton <proton@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 13:06:15 by proton            #+#    #+#             */
/*   Updated: 2025/09/02 11:59:45 by proton           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FILLBODYINCREATEDFILE_HPP
#define FILLBODYINCREATEDFILE_HPP

#include "Request.hpp"
#include <fstream>
#include <string>
#include <map>

int fillBodyJpeg( Request &requestInstance, std::string& body, std::string& filepath );
int fillBodyTextPlain( Request &requestInstance, std::string& body, std::string& filepath );
int fillBodyWwwFormUrlEncoded( Request &requestInstance, Request &tempRequest, std::string& filepath );

#endif