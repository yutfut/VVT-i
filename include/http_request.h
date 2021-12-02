//
// Created by Yut Fut on 18.11.2021.
//

#ifndef VVT_I_HTTP_REQEST_H
#define VVT_I_HTTP_REQEST_H

#include <errno.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>       // gethostbyname
#include <netinet/in.h>  // struct sockaddr_in
#include <string>

class HTTPRequest {
public:
    static int send();
};

#endif //VVT_I_HTTP_REQEST_H