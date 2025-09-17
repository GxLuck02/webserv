/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RecieveCgi.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bproton <bproton@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 11:31:11 by proton            #+#    #+#             */
/*   Updated: 2025/09/17 15:47:03 by bproton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/RecieveCgi.hpp"

static char **makeEnv(Request &requestInstance)
{
    std::string methode = requestInstance.getMethode();
    char        **myEnv;
    
    if (methode == "GET")
    {
        // SCRIPT_NAME
        // REQUEST_METHODE
        // SERVER NAME
        // SERVER_PORT
    }

    else if (methode == "POST")
    {
        // CONTENT_LENGTH
        // CONTENT_TYPE
        // SCRIPT_NAME
        // REQUEST_METHODE
        // SERVER NAME
        // SERVER_PORT
    }

    else
    {
        // SCRIPT_NAME
        // REQUEST_METHODE
        // SERVER NAME
        // SERVER_PORT
    }

    return (myEnv);
}

int executeCgi(Request &requestInstance, Response &responseInstance, Client &clientInstance)
{
    char **myEnv;

    myEnv = makeEnv(requestInstance);
}