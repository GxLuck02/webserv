/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FillBodyInCreatedFIle.hpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: proton <proton@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 13:06:15 by proton            #+#    #+#             */
/*   Updated: 2025/08/15 13:58:55 by proton           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FILLBODYINCREATEDFILE_HPP
#define FILLBODYINCREATEDFILE_HPP

#include "Request.hpp"
#include <fstream>
#include <string>
#include <map>

int fillBodyJpeg( Request &requestInstance );
int fillBodyTextPlain( Request &requestInstance );
int fillBodyWwwFormUrlEncoded( Request &requestInstance, const Request &tempRequest );

#endif