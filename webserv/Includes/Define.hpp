/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Define.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttreichl <ttreichl@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 15:32:13 by ttreichl          #+#    #+#             */
/*   Updated: 2025/08/20 15:32:18 by ttreichl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEFINE_HPP
#define DEFINE_HPP

//default configuration values
#define DEFAUT_CONF "default.conf"
#define DECIMAL				"0123456789"
#define SUFIX_BYTES			"bBkKmMgG"
#define MAX_SIZE_LIMIT		10737418240.0	// 10GB
#define TIMEOUT_DEF			60

#define ERR_MAX_SIZE_INPUT(size)		"'" + size + "' is not a valid size. Size must be a number positive or a number followed by a sufix (b - B, k - K, m - M, g - G)"
#define ERR_MAX_SIZE_RANGE(size)		"'" + size + "' is not a valid size. The max value allowed is 10G (10737418240 bytes)"


#endif