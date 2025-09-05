/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParseJpeg.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: proton <proton@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 17:20:40 by proton            #+#    #+#             */
/*   Updated: 2025/08/07 18:01:23 by proton           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/ParseJpeg.hpp"

int parseJpeg( Request& requestInstance, const std::string &jpegData )
{
	if (jpegData.empty())
	{
		requestInstance.setStatusCode(400);
		requestInstance.setErrorBody("JPEG data is empty");
		return -1;
	}

	if (jpegData.size() < 3 || jpegData[0] != (char)0xFF || jpegData[1] != (char)0xD8 || jpegData[2] != (char)0xFF)
	{
		requestInstance.setStatusCode(400);
		requestInstance.setErrorBody("Invalid JPEG data");
		return -1;
	}
	return (0);
}
