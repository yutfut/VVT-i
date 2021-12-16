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

extern const char ip_address[];
const int host = 3000;
const int line_size = 1024;

class HTTPBase {
public:
    static std::string send(const std::string &message);
};

#endif //VVT_I_HTTP_REQEST_H