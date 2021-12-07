//
// Created by Yut Fut on 18.11.2021.
//

#ifndef VVT_I_COMMAND_H
#define VVT_I_COMMAND_H

#include <iostream>
#include <string>

#include <stdio.h>
#include <regex>

#include "validation.h"
#include "http_request.h"

class Command {
public:

    static int search_file(const std::string& file_name);

    static int upload(const std::string& command);

    static int download(const std::string& command);

//    static int work_with_chmod(const std::string& command);
//
//    static int work_with_directory(const std::string& command);
};

#endif //VVT_I_COMMAND_H
