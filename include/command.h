//
// Created by Yut Fut on 18.11.2021.
//

#ifndef VVT_I_COMMAND_H
#define VVT_I_COMMAND_H

#include <iostream>
#include <string>
#include <cstring>

#include "validation.h"
#include "http_base.h"
#include "http_request.h"
#include "http_response.h"

class Command {
public:
    static std::string input_password();

    static std::string check_password_upload();

    static std::string check_password_download();

    static int upload(const std::string& command);

    static int download(const std::string& command);
};

#endif //VVT_I_COMMAND_H
