//
// Created by Yut Fut on 18.11.2021.
//

#ifndef VVTI_CLIENT_CLIENT_H
#define VVTI_CLIENT_CLIENT_H

#include <map>

#include "command.h"

class Client {
private:
    std::map <std::string, int> commands;

    enum Commands {
        UPLOAD,
        DOWNLOAD,
        REGISTER,
        LOGIN,
        LOGOUT,
        EXIT,
    };

    enum Role {
        SUCCESS,
        GOODBYE,
    };
public:
    Client();

    ~Client();

    void print();

    int role_command(const std::string& command, User &user);

    void run();
};

#endif //VVTI_CLIENT_CLIENT_H
