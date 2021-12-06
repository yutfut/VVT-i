//
// Created by Yut Fut on 18.11.2021.
//

#include "http_request.h"

void HTTPRequest::init_sockaddr(struct sockaddr_in* name) {
    name->sin_family = AF_INET;
    name->sin_port = htons(3000);
    inet_pton(AF_INET, "127.0.0.1", &(name->sin_addr));

//    struct hostent* hostinfo = gethostbyname("vk.com");
//    if (hostinfo == NULL) {
//        fprintf(stderr, "Unknown host %s.\n", "vk.com");
//        exit(EXIT_FAILURE);
//    }
//    name->sin_addr = *(struct in_addr*)hostinfo->h_addr;
}

void HTTPRequest::crate_message(std::string &file_name, std::string &message) {
    std::string buff;
    std::ifstream in(file_name);

    if (in.is_open())
    {
        while (getline(in, buff))
        {
            message.append(buff.append("\n"));

        }
    }

    in.close();
}

void HTTPRequest::write_to_server(int filedes, std::string msg) {
    size_t left = msg.size();
    ssize_t sent = 0;

    int flags = 0;
    while (left > 0) {
        sent = ::send(filedes, msg.data() + sent, msg.size() - sent, flags);
        if (-1 == sent)
            throw std::runtime_error("write failed: " + std::string(strerror(errno)));

        left -= sent;
    }

    std::cout << msg << std::endl;
}

void HTTPRequest::read_from_server(int filedes) {
    char buf[1024];
#ifdef __APPLE__
    int n = ::recv(filedes, buf, sizeof(buf), 0);
#else
    int n = ::recv(filedes, buf, sizeof(buf), MSG_NOSIGNAL);
#endif

    if (-1 == n && errno != EAGAIN)
        throw std::runtime_error("read failed: " + std::string(strerror(errno)));
    if (0 == n)
        throw std::runtime_error("client: " + std::to_string(filedes) +
                                 " disconnected");
    if (-1 == n)
        throw std::runtime_error("client: " + std::to_string(filedes) +
                                 " timeouted");

    std::string ret(buf, buf + n);
    std::cerr << "client: " << filedes << ", recv: \n"
              << ret << " [" << n << " bytes]" << std::endl;
}


int HTTPRequest::send(std::string file_name) {


    int fd = socket(PF_INET, SOCK_STREAM, 0);
    if (fd <= 0) {
        throw std::runtime_error(std::string(strerror(errno)));
    }

    struct sockaddr_in server;
    init_sockaddr(&server);

    if (connect(fd, (struct sockaddr*)&server, sizeof(server)) != 0) {
        close(fd);
//        throw std::runtime_error(std::string(strerror(errno)));
        std::cout << "ошибка соединения\n";
    }

    std::string msg, buff, message="";
    msg = "email: …\n"
          "password: …\n"
          "status: “status”/“error”\n"
          "message: …\n"
          "command: upload/download\n"
          "filename: \n\n";
    crate_message(file_name, message);
    msg.append(message);

    write_to_server(fd, msg);
    read_from_server(fd);

    close(fd);

    return 0;
}
