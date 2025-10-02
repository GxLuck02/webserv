/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParseWwwFormUrlEncoded.hpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: proton <proton@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 11:04:17 by proton            #+#    #+#             */
/*   Updated: 2025/09/04 18:08:08 by proton           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSEWWWFORMURLENCODED_HPP
#define PARSEWWWFORMURLENCODED_HPP

#include "Request.hpp"
#include <fstream>

int parseWwwFormUrlEncoded(Request &requestInstance, const std::string &body);

#endif