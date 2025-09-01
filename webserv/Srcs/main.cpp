/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttreichl <ttreichl@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 16:38:44 by ttreichl          #+#    #+#             */
/*   Updated: 2025/08/25 15:29:33 by ttreichl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <csignal>

Server* g_server = NULL;

void handle_sigint(int signum) {
    std::cout << "\n📢 SIGINT reçu (Ctrl+C), fermeture propre...\n";
    if (g_server) {
        delete g_server;
        g_server = NULL;
    }
    std::exit(signum);
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <config_file_path>\n";
        return 1;
    }
    signal(SIGINT, handle_sigint);
    try
    {
        g_server = new Server(argv[1]);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return 1;
    }
    try {
        g_server->initServer();
        g_server->run();
    } catch (const std::exception &e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        delete g_server;
        return 1;
    }
    delete g_server;
    return 0;
}
