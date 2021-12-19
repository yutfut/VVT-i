//
// Created by Yut Fut on 18.11.2021.
//

#include "client.h"

Client::Client(){}

Client::~Client(){}

void Client::print() {
    std::cout << ">>>\t";
}

int Client::role_command(const std::string& command, User &user) {
    size_t pos = command.find_first_of(' ');
    std::string first_part_command = command.substr(0, pos);
    std::string rest_part_command = command.substr(pos + 1);

    switch (commands[first_part_command]) {
        case UPLOAD: {
            if(first_part_command != rest_part_command) {
                return upload(rest_part_command, user);
            }
            std::cout << "Ошибка ввода команды" << std::endl;
            return -1;
        }
        case DOWNLOAD: {
            if(first_part_command != rest_part_command) {
                return download(rest_part_command, user);
            }
            std::cout << "Ошибка ввода команды" << std::endl;
            return -1;
        }
        case REGISTER: {
            if(first_part_command != rest_part_command) {
                return user.register_user(rest_part_command);
            }
            std::cout << "Ошибка ввода команды" << std::endl;
            return -1;
        }
        case LOGIN: {
            if(first_part_command != rest_part_command) {
                return user.login(rest_part_command);
            }
            std::cout << "Ошибка ввода команды" << std::endl;
            return -1;
        }
        case LOGOUT: {
            return user.logout();
        }
        case DIR: {
            if(first_part_command != rest_part_command) {
                return work_with_directory(first_part_command, rest_part_command, user);
            }
            std::cout << "Ошибка ввода команды" << std::endl;
            return -1;
        }
        case EXIT: {
            return 1;
        }
        default: {
            std::cout << "Команды не существует" << std::endl;
            return -1;
        }
    }
}

void Client::run() {
    commands["upload"] = UPLOAD;
    commands["download"] = DOWNLOAD;
    commands["register"] = REGISTER;
    commands["login"] = LOGIN;
    commands["logout"] = LOGOUT;
    commands["cd"] = DIR;
    commands["ls"] = DIR;
    commands["mkdir"] = DIR;
    commands["rmdir"] = DIR;
    commands["exit"] = EXIT;

    User user;

    std::cout << "Welcome to VVTi!" << std::endl;
    std::string command;

    while (true) {
        print();
        std::getline (std::cin, command);
        if (command.empty()) {
            continue;
        }

        switch (role_command(command, user)){
            case SUCCESS:
                std::cout << "SUCСESS" << std::endl;
                break;
            case GOODBYE:
                std::cout << "GOODBYE" << std::endl;
                return;
            default:
                std::cout << "ERROR" << std::endl;
                break;
        }
    }

}

