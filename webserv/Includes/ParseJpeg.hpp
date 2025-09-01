/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParseJpeg.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: proton <proton@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 17:20:59 by proton            #+#    #+#             */
/*   Updated: 2025/08/07 17:26:11 by proton           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSEJPEG_HPP
#define PARSEJPEG_HPP

#include <iostream>
#include <string>
#include "Request.hpp"

int parseJpeg( Request& requestInstance, const std::string &jpegData );


#endif