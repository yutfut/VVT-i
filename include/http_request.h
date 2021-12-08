//
// Created by Yut Fut on 18.11.2021.
//

// http base

#ifndef VVT_I_HTTP_REQEST_H
#define VVT_I_HTTP_REQEST_H

#include "command.h"

#include <errno.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string>
#include <cstring>
#include <fstream>
#include <iostream>

class HTTPRequest {
public:
    static void init_socket_address(struct sockaddr_in &name);

    static int request(const int &socket, const std::string msg);

    static std::string response(const int &socket);

    static std::string send(const std::string &message);
};

#endif //VVT_I_HTTP_REQEST_H