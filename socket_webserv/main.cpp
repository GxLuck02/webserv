/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttreichl <ttreichl@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 16:38:44 by ttreichl          #+#    #+#             */
/*   Updated: 2025/07/02 18:33:32 by ttreichl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <csignal>
#include <iostream>
#include <cstdlib>

Server* g_server = NULL;

void handle_sigint(int signum) {
    std::cout << "\n📢 SIGINT reçu (Ctrl+C), fermeture propre...\n";
    if (g_server) {
        delete g_server;
        g_server = NULL;
    }
    std::exit(signum);
}

int main() {
    signal(SIGINT, handle_sigint);

    g_server = new Server();
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

