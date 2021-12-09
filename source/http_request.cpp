//
// Created by Yut Fut on 18.11.2021.
//

#include "http_request.h"

void HTTPRequest::init_socket_address(struct sockaddr_in &server) {
    server.sin_family = AF_INET;
    server.sin_port = htons(3000);
    server.sin_port = htons(80);
    inet_pton(AF_INET, "127.0.0.1", &(server.sin_addr));
}

int HTTPRequest::request(const int &socket, const std::string msg) {
    size_t left = msg.size();
    ssize_t sent = 0;

    while (left > 0) {
        sent = ::send(socket, msg.data() + sent, msg.size() - sent, 0);
        if (sent == -1) {
            std::cout << "ошибка соединения\n";
            return -1;
        }
        left -= sent;
    }

    std::cout << msg << std::endl;
    return 0;
}

std::string HTTPRequest::response(const int &socket) {
    char buf[1024];
#ifdef __APPLE__
    int n = ::recv(socket, buf, sizeof(buf), 0);
#else
    int n = ::recv(socket, buf, sizeof(buf), MSG_NOSIGNAL);
#endif

    if (-1 == n && errno != EAGAIN) {
        return "ошибка соединения\n";
    }
    if (0 == n) {
        return "ошибка соединения\n";
    }
    if (-1 == n) {
        return "ошибка соединения\n";
    }

    std::string ret(buf, buf + n);
    std::cerr << ret << " [" << n << " bytes]" << std::endl;
    return ret;
}

std::string HTTPRequest::send(const std::string &message) {


    int client_socket = socket(PF_INET, SOCK_STREAM, 0);
    if (client_socket <= 0) {
        std::cout << "ошибка соединения\n";
        return "ошибка соединения\n";
    }

    struct sockaddr_in server;
    init_socket_address(server);

    if (connect(client_socket, (struct sockaddr*)&server, sizeof(server)) != 0) {
        close(client_socket);
        return "ошибка соединения\n";
    }

    if (request(client_socket, message)) {
        return "ошибка соединения\n";
    }


    std::string http_response = response(client_socket);
    if (http_response == "ошибка соединения\n") {
        std::cout << "ошибка соединения\n";
        return "ошибка соединения\n";
    }

    close(client_socket);

    return http_response;
}
