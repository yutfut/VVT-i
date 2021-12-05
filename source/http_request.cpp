//
// Created by Yut Fut on 18.11.2021.
//

#include "http_request.h"


int HTTPRequest::send(std::string file_name) {


    int fd = socket(PF_INET, SOCK_STREAM, 0);
    if (fd <= 0) {
        throw std::runtime_error(std::string(strerror(errno)));
    }

    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(8000);
    inet_pton(AF_INET, "127.0.0.1", &(server.sin_addr));

    if (connect(fd, (struct sockaddr*)&server, sizeof(server)) != 0) {
        close(fd);
//        throw std::runtime_error(std::string(strerror(errno)));
        std::cout << "ошибка соединения\n";
    }

    std::string msg = "Server: VVT-i\n", buff, message="";
//    FILE *fp;

//    if((fp=fopen(file_name.c_str(), "rb+")) == NULL) {
//            std::cout << "Cannot open file.\n";
//    }
//
//    while(fscanf(fp, "%d\n", &buff) != EOF) {
//        std::cout << buff << std::endl;
//        msg += buff;
//    }
//    fclose(fp);

    std::ifstream in(file_name);

    if (in.is_open())
    {
        while (getline(in, buff))
        {
            std::cout << buff << std::endl;
            message.append(buff.append("\n"));

        }
    }
    std::cout << message << std::endl;

    msg.append(message);

    in.close();



    std::cout << msg << std::endl;



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
//
//    char buf[128];
//
//    int n = ::recv(fd, buf, sizeof(buf), /*flags*/0);
//    if (-1 == n) {
//        throw std::runtime_error(std::string(strerror(errno)));
//    }
//
//    std::string ret(buf, buf + n);

    int a = close(fd);

    return 0;
}
