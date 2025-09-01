/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParseWwwFormUrlEncoded.hpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: proton <proton@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 11:04:17 by proton            #+#    #+#             */
/*   Updated: 2025/08/07 10:49:36 by proton           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSEWWWFORMURLENCODED_HPP
#define PARSEWWWFORMURLENCODED_HPP

#include "Request.hpp"

int parseWwwFormUrlEncoded(Request &requestInstance, const std::string &body);

#endif