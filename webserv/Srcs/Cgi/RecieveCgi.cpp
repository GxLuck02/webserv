/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RecieveCgi.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bproton <bproton@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 11:31:11 by proton            #+#    #+#             */
/*   Updated: 2025/09/18 15:35:41 by bproton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/RecieveCgi.hpp"

static char **makeEnv(Request &requestInstance, Client &clientInstance)
{
    char    **myEnv;
    int contentLength = requestInstance.getContentLength();
    std::string methode = requestInstance.getMethode();
    std::string serverName = requestInstance.getField("Host");
    int port = clientInstance.getServConfig()->getPort();
    std::stringstream ss;
    std::stringstream sslength;
    sslength << contentLength;
    ss << port;
    
    if (methode == "GET")
    {
        myEnv = new char*[10];
        myEnv[0] = new char[16 + methode.length() + 1]; // 15 is REQUEST_METHODE=
        std::strcpy(myEnv[0], ("REQUEST_METHODE=" + methode + '\0').c_str());
        myEnv[1] = new char[12 + requestInstance.getUri().length() + 1];
        std::strcpy(myEnv[1], ("SCRIPT_NAME=" + requestInstance.getUri() + '\0').c_str());
        myEnv[2] = new char[16 + requestInstance.getHttpVersion().length() + 1];
        std::strcpy(myEnv[2], ("SERVER_PROTOCOL=" + requestInstance.getHttpVersion() + '\0').c_str());
        myEnv[3] = new char[12 + serverName.length() + 1];
        std::strcpy(myEnv[3], ("SERVER_NAME=" + serverName + '\0').c_str());
        myEnv[4] = new char[12 + ss.str().length() + 1];
        std::strcpy(myEnv[4], ("SERVER_PORT=" + ss.str() + '\0').c_str());
        myEnv[5] = new char[13 + requestInstance.getQuery().length() + 1];
        std::strcpy(myEnv[5], ("QUERY_STRING=" + requestInstance.getQuery() + '\0').c_str());
        myEnv[6] = new char[16];
        std::strcpy(myEnv[6], "CONTENT_LENGTH=\0");
        myEnv[7] = new char[14];
        std::strcpy(myEnv[7], "CONTENT_TYPE=\0");
        myEnv[8] = new char[26]; // GATEWAY_INTERFACE
        std::strcpy(myEnv[8], "GATEWAY_INTERFACE=CGI/1.1\0");
        myEnv[9] = NULL;
    }

    else if (methode == "POST")
    {
        myEnv = new char*[10];
        myEnv[0] = new char[16 + methode.length() + 1]; // 15 is REQUEST_METHODE=
        std::strcpy(myEnv[0], ("REQUEST_METHODE=" + methode + '\0').c_str());
        myEnv[1] = new char[12 + requestInstance.getUri().length() + 1];
        std::strcpy(myEnv[1], ("SCRIPT_NAME=" + requestInstance.getUri() + '\0').c_str());
        myEnv[2] = new char[16 + requestInstance.getHttpVersion().length() + 1];
        std::strcpy(myEnv[2], ("SERVER_PROTOCOL=" + requestInstance.getHttpVersion() + '\0').c_str());
        myEnv[3] = new char[12 + serverName.length() + 1];
        std::strcpy(myEnv[3], ("SERVER_NAME=" + serverName + '\0').c_str());
        myEnv[4] = new char[12 + ss.str().length() + 1];
        std::strcpy(myEnv[4], ("SERVER_PORT=" + ss.str() + '\0').c_str());
        myEnv[5] = new char[13 + requestInstance.getQuery().length() + 1];
        std::strcpy(myEnv[5], ("QUERY_STRING=" + requestInstance.getQuery() + '\0').c_str());
        myEnv[6] = new char[15 + sslength.str().length() + 1];
        std::strcpy(myEnv[6], ("CONTENT_LENGTH=" + sslength.str() + '\0').c_str());
        myEnv[7] = new char[13 + requestInstance.getContentType().length() + 1];
        std::strcpy(myEnv[7], ("CONTENT_TYPE=" + requestInstance.getContentType() + '\0').c_str());
        myEnv[8] = new char[26]; // GATEWAY_INTERFACE
        std::strcpy(myEnv[8], "GATEWAY_INTERFACE=CGI/1.1\0");
        myEnv[9] = NULL;
    }

    else
    {
        myEnv = new char*[10];
        myEnv[0] = new char[16 + methode.length() + 1]; // 15 is REQUEST_METHODE=
        std::strcpy(myEnv[0], ("REQUEST_METHODE=" + methode + '\0').c_str());
        myEnv[1] = new char[12 + requestInstance.getUri().length() + 1];
        std::strcpy(myEnv[1], ("SCRIPT_NAME=" + requestInstance.getUri() + '\0').c_str());
        myEnv[2] = new char[16 + requestInstance.getHttpVersion().length() + 1];
        std::strcpy(myEnv[2], ("SERVER_PROTOCOL=" + requestInstance.getHttpVersion() + '\0').c_str());
        myEnv[3] = new char[12 + serverName.length() + 1];
        std::strcpy(myEnv[3], ("SERVER_NAME=" + serverName + '\0').c_str());
        myEnv[4] = new char[12 + ss.str().length() + 1];
        std::strcpy(myEnv[4], ("SERVER_PORT=" + ss.str() + '\0').c_str());
        myEnv[5] = new char[13 + requestInstance.getQuery().length() + 1];
        std::strcpy(myEnv[5], ("QUERY_STRING=" + requestInstance.getQuery() + '\0').c_str());
        myEnv[6] = new char[16];
        std::strcpy(myEnv[6], "CONTENT_LENGTH=\0");
        myEnv[7] = new char[14];
        std::strcpy(myEnv[7], "CONTENT_TYPE=\0");
        myEnv[8] = new char[26]; // GATEWAY_INTERFACE
        std::strcpy(myEnv[8], "GATEWAY_INTERFACE=CGI/1.1\0");
        myEnv[9] = NULL;
    }

    return (myEnv);
}

int executeCgi(Request &requestInstance, Response &responseInstance, Client &clientInstance)
{
    char        **myEnv;
    int         fd[2];
    pid_t       pid;
    int         status;
    int         bytesRead = -1;
    char        buffer[BUFFER_SIZE + 1];
    std::string body;

    myEnv = makeEnv(requestInstance, clientInstance);
    
    if (pipe(fd) == -1)
    {
        requestInstance.setStatusCode(500);
        requestInstance.setErrorBody("Internal Server Error");
        return (-1);
    }

    pid = fork();
    if (pid == -1)
    {
        requestInstance.setStatusCode(500);
        requestInstance.setErrorBody("Internal Server Error");
        return (-1);
    }
    else if (pid == 0)
    {
        close(fd[0]);
        dup2(fd[1], STDOUT_FILENO);
        close(fd[1]);
        execve(requestInstance.getUri().c_str(), NULL, myEnv);
        exit(1);
    }
    else
    {
        close(fd[1]);
        waitpid(pid, &status, 0);
        if (WIFEXITED(status))
        {
            while (bytesRead != 0)
            {
                bytesRead = read(fd[0], buffer, BUFFER_SIZE);
                if (bytesRead == -1)
                {
                    close(fd[0]);
                    requestInstance.setStatusCode(500);
                    requestInstance.setErrorBody("Internal Server Error");
                    return (-1);
                }
                body += (std::string)buffer;
            }
            if (requestInstance.getMethode() == "POST")
                responseInstance.setStatusCode(201);
            else
                responseInstance.setStatusCode(200);
            responseInstance.setContentType("text/html");
            responseInstance.setContentLength(body.length());
            responseInstance.setBody(body);
        }
        else
        {
            close(fd[0]);
            requestInstance.setStatusCode(502);
            requestInstance.setErrorBody("Bad Gateway");
            return (-1);
        }
        return (0);
    }
    
}