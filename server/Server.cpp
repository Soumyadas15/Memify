#include "Server.h"
#include "ConnectionHandler.h"
#include "INIReader.h"
#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>
#include <thread>

Server::Server(std::shared_ptr<ICache> cache, uint16_t port)
    : cache_(std::move(cache)), port_(port), secret_key_("xyz"), running_(false) 
{
    INIReader reader("../config.ini");

    if (reader.ParseError() < 0) {
        std::cerr << "Can't load 'config.ini'" << std::endl;
        return;
    }

    port_ = reader.GetInteger("settings", "port", port_);
    secret_key_ = reader.Get("settings", "secret_key", secret_key_);
}

void Server::Start() {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        std::cerr << "Socket creation failed" << std::endl;
        return;
    }

    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        std::cerr << "setsockopt(SO_REUSEADDR) failed" << std::endl;
        close(server_fd);
        return;
    }

    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(static_cast<uint16_t>(port_));
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Bind failed" << std::endl;
        close(server_fd);
        return;
    }

    if (listen(server_fd, SOMAXCONN) < 0) {
        std::cerr << "Listen failed" << std::endl;
        close(server_fd);
        return;
    }

    std::cout << "Memify is listening on port " << port_ << std::endl;

    running_ = true;
    while (running_) {
        sockaddr_in client_addr{};
        socklen_t client_addr_len = sizeof(client_addr);
        int client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_addr_len);
        if (client_fd < 0) {
            std::cerr << "Accept failed" << std::endl;
            continue;
        }

        std::cout << "Client connected" << std::endl;
        std::thread(&ConnectionHandler::HandleClient, ConnectionHandler(cache_, client_fd, secret_key_)).detach();
    }

    close(server_fd);
}

void Server::Stop() {
    running_ = false;
}