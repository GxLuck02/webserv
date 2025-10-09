/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FillBodyInCreatedFile.hpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: proton <proton@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 13:06:15 by proton            #+#    #+#             */
/*   Updated: 2025/10/08 17:41:55 by proton           ###   ########.fr       */
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

#endif