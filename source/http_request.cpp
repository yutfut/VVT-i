//
// Created by Yut Fut on 18.11.2021.
//

#include "http_request.h"

void HTTPRequest::init_socket_address(struct sockaddr_in &name) {
    name.sin_family = AF_INET;
//    name->sin_port = htons(3000);
    name.sin_port = htons(80);

//    inet_pton(AF_INET, "127.0.0.1", &(name->sin_addr));

    struct hostent* hostinfo = gethostbyname("vk.com");
    if (hostinfo == NULL) {
        fprintf(stderr, "Unknown host %s.\n", "vk.com");
        exit(EXIT_FAILURE);
    }
    name.sin_addr = *(struct in_addr*)hostinfo->h_addr;
}

int HTTPRequest::request(const int &socket, const std::string msg) {
    size_t left = msg.size();
    ssize_t sent = 0;

    int flags = 0;
    while (left > 0) {
        sent = ::send(socket, msg.data() + sent, msg.size() - sent, flags);
        if (-1 == sent) {
//            throw std::runtime_error("write failed: " + std::string(strerror(errno)));
            std::cout << "ошибка соединения\n";
            return -1;
            break;
        }

        left -= sent;
    }

    std::cout << msg << std::endl;
    return 0;
}

int HTTPRequest::response(const int &socket) {
    char buf[1024];
#ifdef __APPLE__
    int n = ::recv(socket, buf, sizeof(buf), 0);
#else
    int n = ::recv(client_socket, buf, sizeof(buf), MSG_NOSIGNAL);
#endif

    if (-1 == n && errno != EAGAIN) {
//        throw std::runtime_error("read failed: " + std::string(strerror(errno)));
        std::cout << "ошибка соединения\n";
        return -1;
    }
    if (0 == n) {
//        throw std::runtime_error("client: " + std::to_string(filedes) +
//                                 " disconnected");
        std::cout << "ошибка соединения\n";
        return -1;
    }
    if (-1 == n) {
//        throw std::runtime_error("client: " + std::to_string(filedes) +
//                                 " timeouted");
        std::cout << "ошибка соединения\n";
        return -1;
    }

    std::string ret(buf, buf + n);
    std::cerr << "client: " << socket << ", recv: \n"
              << ret << " [" << n << " bytes]" << std::endl;
    return 0;
}

int HTTPRequest::send(const std::string &message) {


    int client_socket = socket(PF_INET, SOCK_STREAM, 0);
    if (client_socket <= 0) {
//        throw std::runtime_error(std::string(strerror(errno)));
        std::cout << "ошибка соединения\n";
        return -1;
    }

    struct sockaddr_in server;
    init_socket_address(server);

    if (connect(client_socket, (struct sockaddr*)&server, sizeof(server)) != 0) {
        close(client_socket);
//        throw std::runtime_error(std::string(strerror(errno)));
        std::cout << "ошибка соединения\n";
        return -1;
    }

    if (request(client_socket, message)) {
        return -1;
    }
    if (response(client_socket)) {
        return -1;
    }

    close(client_socket);

    return 0;
}