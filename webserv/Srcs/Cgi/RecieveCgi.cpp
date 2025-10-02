/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RecieveCgi.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttreichl <ttreichl@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 17:55:37 by proton            #+#    #+#             */
/*   Updated: 2025/10/02 11:49:01 by ttreichl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/RecieveCgi.hpp"
#include <ctime>
#include <signal.h>
#include <unistd.h>

int handlePostCgi(Request &requestInstance, Response &responseInstance, Client &clientInstance)
{
    if (fillContentLength(requestInstance, responseInstance) == -1)
        return (-1);

    std::cout << "***************  Content lenght size: " << requestInstance.getContentLength() << std::endl;
    std::cout << "Max body size: " << clientInstance.getServConfig()->getMaxBodySize() << std::endl;
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
    std::string query = requestInstance.getQuery();
    std::stringstream port;
    std::stringstream contentLength;
    port << clientInstance.getServConfig()->getPort();
    if (requestInstance.getContentLength() == 0)
        contentLength << "";
    else
        contentLength << requestInstance.getContentLength();

    if (methods == "GET" || methods == "DELETE")
    {
        std::cout << "BEFORE MAKE ENV GET AND DELETE" << std::endl;
        envVars.push_back("REQUEST_METHOD=" + methods);
        envVars.push_back("SCRIPT_NAME=" + requestInstance.getUri());
        envVars.push_back("SERVER_PROTOCOL=" + requestInstance.getHttpVersion());
        envVars.push_back("SERVER_NAME=" + clientInstance.getServConfig()->getServName());
        envVars.push_back("SERVER_PORT=" + port.str());
        if (query.empty())
            envVars.push_back("QUERY_STRING=");
        else
            envVars.push_back("QUERY_STRING=" + requestInstance.getQuery());
        envVars.push_back("CONTENT_LENGTH=");
        envVars.push_back("CONTENT_TYPE=");
        envVars.push_back("GATEWAY_INTERFACE=CGI/1.1");
        std::cout << "AFTER MAKE ENV GET AND DELETE" << std::endl;
    }
    else if (methods == "POST")
    {
        std::cout << "BEFORE MAKE ENV POST" << std::endl;
        envVars.push_back("REQUEST_METHOD=" + methods);
        envVars.push_back("SCRIPT_NAME=" + requestInstance.getUri());
        envVars.push_back("SERVER_PROTOCOL=" + requestInstance.getHttpVersion());
        envVars.push_back("SERVER_NAME=" + clientInstance.getServConfig()->getServName());
        envVars.push_back("SERVER_PORT=" + port.str());
        if (query.empty())
            envVars.push_back("QUERY_STRING=");
        else
            envVars.push_back("QUERY_STRING=" + requestInstance.getQuery());
        envVars.push_back("CONTENT_LENGTH=" + contentLength.str());
        envVars.push_back("CONTENT_TYPE=" + requestInstance.getContentType());
        envVars.push_back("GATEWAY_INTERFACE=CGI/1.1");
        std::cout << "AFTER MAKE ENV POST" << std::endl;
    }


    myEnv = new char*[envVars.size() + 1];
    std::cout << "ASSIGNING ENV BEFORE" << std::endl;
    for (size_t i = 0; i < envVars.size(); ++i) {
        myEnv[i] = new char[envVars[i].size() + 1];
        std::strcpy(myEnv[i], envVars[i].c_str());
    }
    std::cout << "ASSIGNING ENV AFTER" << std::endl;

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
    std::string root = "." + requestInstance.getUri().substr(1, requestInstance.getUri().length() -1);
    char *newRoot = const_cast<char *>(root.c_str());

    if (requestInstance.getMethode() == "POST")
    {
        if (handlePostCgi(requestInstance, responseInstance, clientInstance) == -1)
            return (-1);
    }

    myEnv = makeEnv(requestInstance, clientInstance);
    
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

        // Gestion du timeout CGI
        int cgiTimeout = clientInstance.getServConfig()->getCgiTimeout();
        time_t startTime = time(NULL);
        bool timedOut = false;
        
        std::cout << "CGI timeout set to: " << cgiTimeout << " seconds" << std::endl;
        
        // Attendre le processus avec timeout
        while (true)
        {
            int result = waitpid(pid, &status, WNOHANG);
            
            if (result > 0)
            {
                // Le processus s'est terminé
                std::cout << "CGI process finished normally" << std::endl;
                break;
            }
            else if (result == -1)
            {
                // Erreur
                std::cout << "CGI waitpid error" << std::endl;
                break;
            }
            
            // Vérifier le timeout
            if (time(NULL) - startTime >= cgiTimeout)
            {
                std::cout << "CGI timeout reached! Killing process..." << std::endl;
                kill(pid, SIGKILL);
                waitpid(pid, &status, 0); // Nettoyer le processus zombi
                timedOut = true;
                break;
            }
            
            // Attendre un peu avant de vérifier à nouveau
            usleep(100000); // 100ms
        }
        std::cout << "CGI process exited with status: " << status << std::endl;
        std::cout << "WIFEXITED: " << WIFEXITED(status) << std::endl;
        std::cout << "WEXITSTATUS: " << WEXITSTATUS(status) << std::endl;
        
        if (timedOut)
        {
            std::cout << "CGI timed out - returning 504 Gateway Timeout" << std::endl;
            close(out_fd[0]);
            requestInstance.setStatusCode(504);
            requestInstance.setErrorBody("Gateway Timeout");
            freeEnv(myEnv);
            return (-1);
        }
        else if (WIFEXITED(status))
        {
            std::cout << "Reading CGI output..." << std::endl;
            while ((bytesRead = read(out_fd[0], buffer, BUFFER_SIZE)) > 0)
            {
                std::cout << "Read " << bytesRead << " bytes from CGI" << std::endl;
                body.append(buffer, bytesRead);
            }
            close(out_fd[0]);
            //std::cout << "CGI body length: " << body.length() << std::endl;
            //std::cout << "CGI body content: [" << body << "]" << std::endl;
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