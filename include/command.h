//
// Created by Yut Fut on 18.11.2021.
//

#ifndef VVT_I_COMMAND_H
#define VVT_I_COMMAND_H

#include "password.h"
#include "http_base.h"
#include "http_request.h"
#include "http_response.h"

int upload(const std::string& command, User &user);

int download(const std::string& command, User &user);

int work_with_directory(const std::string& first_part_command, const std::string& rest_part_command, User &user);



#endif //VVT_I_COMMAND_H
