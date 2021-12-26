//
// Created by Yut Fut on 18.11.2021.
//

#ifndef VVTI_CLIENT_CLIENT_H
#define VVTI_CLIENT_CLIENT_H

#include <map>

#include "command.h"

const std::string UPLOAD = "upload";
const std::string DOWNLOAD = "download";
const std::string REGISTER = "register";
const std::string LOGIN = "login";
const std::string LOGOUT = "logout";
const std::string CD = "cd";
const std::string LS = "ls";
const std::string MKDIR = "mkdir";
const std::string RMDIR = "rmdir";
const std::string EXIT = "exit";

class Client {
private:
    enum class Commands {
        UPLOAD,
        DOWNLOAD,
        REGISTER,
        LOGIN,
        LOGOUT,
        DIR,
        EXIT,
    };

    std::map <std::string, Commands> commands;

    enum class Status {
        SUCCESS,
        GOODBYE,
    };
public:
    Client();

    ~Client();

    void print(User &user);

    int role_command(const std::string& command, User &user);

    void run();
};

#endif //VVTI_CLIENT_CLIENT_H
