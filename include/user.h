//
// Created by Yut Fut on 18.11.2021.
//

#ifndef VVT_I_USER_H
#define VVT_I_USER_H

#include <iostream>
#include <string>
#include <cstring>
#include <utility>

#include "password.h"
#include "http_base.h"
#include "http_request.h"
#include "http_response.h"

class User {
private:
    bool authorize;
    std::string jwt;
    std::string current_directory;
public:
    User();

    ~User();

    bool get_authorize() {
        return authorize;
    }

    std::string get_jwt() {
        return jwt;
    }

    std::string get_current_directory() {
        return current_directory;
    }

    static int register_user(const std::string& command);

    int login(const std::string& command);

    int logout();
};

#endif //VVT_I_USER_H