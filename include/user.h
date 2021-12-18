//
// Created by Yut Fut on 18.11.2021.
//

#ifndef VVT_I_REGISTRATE_H
#define VVT_I_REGISTRATE_H

#include <iostream>
#include <string>
#include <cstring>

#include "password.h"
#include "http_base.h"
#include "http_request.h"
#include "http_response.h"

class User {
public:
    bool authorize;

    std::string jwt;

    User();

    ~User();

    int register_user(const std::string& command);

    int login(const std::string& command);

    int logout();
};

#endif //VVT_I_REGISTRATE_H
