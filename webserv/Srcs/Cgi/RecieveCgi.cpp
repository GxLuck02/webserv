/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RecieveCgi.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: proton <proton@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 17:55:37 by proton            #+#    #+#             */
/*   Updated: 2025/09/22 10:31:05 by proton           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/RecieveCgi.hpp"

int handlePostCgi(Request &requestInstance, Response &responseInstance, Client &clientInstance)
{
    if (fillContentLength(requestInstance, responseInstance) == -1)
        return (-1);

    if (fillContentType(requestInstance, responseInstance) == -1)
        return (-1);

    if (requestInstance.getContentLength() > clientInstance.getServConfig()->getMaxBodySize())
    {
        requestInstance.setStatusCode(413);
        requestInstance.setErrorBody("PayloadTooLarge");
        return (-1);
    }

    if (fillBody(requestInstance, clientInstance.getBuffer(), clientInstance) == -1)
        return (-1);

    return (0);
}

void freeEnv(char **env)
{
    if (!env)
        return;
    for (int i = 0; env[i]; ++i)
        if (env[i])
            delete[] env[i];
    delete[] env;
}

static char **makeEnv(Request &requestInstance, Client &clientInstance)
{
    std::vector<std::string> envVars;
    char **myEnv;
    std::string methods = requestInstance.getMethode();
    std::stringstream port;
    std::stringstream contentLength;
    port << clientInstance.getServConfig()->getPort();
    contentLength << requestInstance.getContentLength();

    std::cout << "BEFORE GET AN DELETE " << std::endl;

    if (methods == "GET" || methods == "DELETE")
    {
        envVars.push_back("REQUEST_METHOD=" + methods);
        envVars.push_back("SCRIPT_NAME=" + requestInstance.getUri());
        envVars.push_back("SERVER_PROTOCOL=" + requestInstance.getHttpVersion());
        envVars.push_back("SERVER_NAME=" + requestInstance.getField("Host"));
        envVars.push_back("SERVER_PORT=" + port.str());
        envVars.push_back("QUERY_STRING=" + requestInstance.getQuery());
        envVars.push_back("CONTENT_LENGTH=");
        envVars.push_back("CONTENT_TYPE=");
        envVars.push_back("GATEWAY_INTERFACE=CGI/1.1");
    }
    else if (methods == "POST")
    {
        envVars.push_back("REQUEST_METHOD=" + methods);
        envVars.push_back("SCRIPT_NAME=" + requestInstance.getUri());
        envVars.push_back("SERVER_PROTOCOL=" + requestInstance.getHttpVersion());
        envVars.push_back("SERVER_NAME=" + requestInstance.getField("Host"));
        envVars.push_back("SERVER_PORT=" + port.str());
        envVars.push_back("QUERY_STRING=" + requestInstance.getQuery());
        envVars.push_back("CONTENT_LENGTH=" + contentLength.str());
        envVars.push_back("CONTENT_TYPE=" + requestInstance.getContentType());
        envVars.push_back("GATEWAY_INTERFACE=CGI/1.1");
    }


    myEnv = new char*[envVars.size() + 1];

    for (size_t i = 0; i < envVars.size(); ++i) {
        myEnv[i] = new char[envVars[i].size() + 1];
        std::strcpy(myEnv[i], envVars[i].c_str());
    }

    myEnv[envVars.size()] = NULL;
    return (myEnv);
}

int handleCgi(Request &requestInstance, Response &responseInstance, Client &clientInstance)
{
    char        **myEnv;
    int         out_fd[2];
    int         in_fd[2];
    pid_t       pid;
    int         status;
    int         bytesRead = -1;
    char        buffer[BUFFER_SIZE + 1];
    std::string body;
    std::string root = "/home/tmontani/42_cursus/webserv/webserv" + requestInstance.getUri().substr(1, requestInstance.getUri().length() -1);
    char *newRoot = const_cast<char *>(root.c_str());

    if (requestInstance.getMethode() == "POST")
    {
        if (handlePostCgi(requestInstance, responseInstance, clientInstance) == -1)
            return (-1);
    }

    myEnv = makeEnv(requestInstance, clientInstance);

    for (size_t i = 0; myEnv[i] != NULL; ++i) {
        std::cout << "argv[" << i << "] = " << myEnv[i] << std::endl;
    }
    
    if (pipe(out_fd) == -1) // pour recuperer la reponse du script
    {
        requestInstance.setStatusCode(500);
        requestInstance.setErrorBody("Internal Server Error");
        freeEnv(myEnv);
        return (-1);
    }
    if (pipe(in_fd) == -1) // pour passer le body au script cgi
    {
        close(out_fd[0]);
        close(out_fd[1]);
        requestInstance.setStatusCode(500);
        requestInstance.setErrorBody("Internal Server Error");
        freeEnv(myEnv);
        return (-1);
    }

    pid = fork();
    if (pid == -1)
    {
        close(out_fd[0]);
        close(out_fd[1]);
        close(in_fd[0]);
        close(in_fd[1]);
        requestInstance.setStatusCode(500);
        requestInstance.setErrorBody("Internal Server Error");
        freeEnv(myEnv);
        return (-1);
    }
    else if (pid == 0)
    {
        close(out_fd[0]);
        close(in_fd[1]);
        if (requestInstance.getMethode() == "POST")
            dup2(in_fd[0], STDIN_FILENO);
        dup2(out_fd[1], STDOUT_FILENO);
        char *args[3] = {const_cast<char *>("/usr/bin/python3"), newRoot, NULL};
        // for (size_t i = 0; args[i] != NULL; i++)
        //     std::cout << "args[" << i << "] = " << args[i] << std::endl;
        execve("/usr/bin/python3", args, myEnv);
        exit(1);
    }
    else
    {
        close(out_fd[1]);
        close(in_fd[0]);
        if (requestInstance.getMethode() == "POST")
        {
            if (write(in_fd[1], requestInstance.getBodyStart().c_str(), requestInstance.getBodyStart().length()) == -1)
            {
                close(in_fd[1]);
                close(out_fd[0]);
                requestInstance.setStatusCode(500);
                requestInstance.setErrorBody("Internal Server Error");
                freeEnv(myEnv);
                return (-1);
            }
            close(in_fd[1]);
        }
        close(in_fd[1]);

        waitpid(pid, &status, 0);
        if (WIFEXITED(status))
        {
            while ((bytesRead = read(out_fd[0], buffer, BUFFER_SIZE)) > 0)
                body.append(buffer, bytesRead);
            close(out_fd[0]);
            if (bytesRead == -1)
            {
                close(out_fd[0]);
                requestInstance.setStatusCode(500);
                requestInstance.setErrorBody("Internal Server Error");
                freeEnv(myEnv);
                return (-1);
            }
            if (parseResponseCgi(requestInstance, responseInstance, body) == -1)
            {
                close(out_fd[0]);
                requestInstance.setStatusCode(500);
                requestInstance.setErrorBody("Internal Server Error");
                freeEnv(myEnv);
                return (-1);
            }
        }
        else
        {
            close(out_fd[0]);
            requestInstance.setStatusCode(502);
            requestInstance.setErrorBody("Bad Gateway");
            freeEnv(myEnv);
            return (-1);
        }
        freeEnv(myEnv);
        close(out_fd[0]);
        return (0);
    }
}