//
// Created by Yut Fut on 18.11.2021.
//

#include "http_request.h"

int HTTPRequest::send() {
    int fd = socket(PF_INET, SOCK_STREAM, 0);
    if (fd <= 0) {
        close(fd);
        throw std::runtime_error(std::string(strerror(errno)));
    }

    struct hostent* hp = gethostbyname("vk.com");

    struct sockaddr_in addr;          // Адрес сервера
    addr.sin_family = AF_INET;        // Для интернет сокетов AF_INET !
    addr.sin_port = htons(80);        // port
    memcpy(&addr.sin_addr, hp->h_addr, hp->h_length);

    int connected = connect(fd, (struct sockaddr*)&addr, sizeof(addr));
    if (connected != 0) {
        close(fd);
        throw std::runtime_error(std::string(strerror(errno)));
    }


    std::string msg = "hello!";
    size_t left = msg.size();
    ssize_t sent = 0;
    int flags = 0;
    while (left > 0) {
        sent = ::send(fd, msg.data() + sent, msg.size() - sent, flags);
        if (-1 == sent) {
            throw std::runtime_error(std::string(strerror(errno)));
        }
        left -= sent;
    }

    char buf[128];

    int n = ::recv(fd, buf, sizeof(buf), /*flags*/0);
    if (-1 == n) {
        throw std::runtime_error(std::string(strerror(errno)));
    }

    std::string ret(buf, buf + n);

    int a = close(fd);

    return 0;
}
