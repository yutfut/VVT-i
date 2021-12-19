//
// Created by Yut Fut on 18.11.2021.
//

#ifndef VVT_I_COMMAND_H
#define VVT_I_COMMAND_H

#include "user.h"
#include "password.h"
#include "http_base.h"
#include "http_request.h"
#include "http_response.h"

int upload(const std::string& command, User &user);
int download(const std::string& command, User &user);

//class Command {
//public:
//    static int upload(const std::string& command);
//
//    static int download(const std::string& command);
//};

#endif //VVT_I_COMMAND_H
