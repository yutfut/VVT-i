//
// Created by Yut Fut on 18.11.2021.
//

#include "client.h"
#include "map"

Client::Client(){}

Client::~Client(){}

void Client::print() {
    std::cout << ">>>\t";
}

enum Commands{
    UPLOAD,
    DOWNLOAD,
    EXIT,
};

std::map <std::string, int> commands;



int Client::role_command(const std::string& command) {


    size_t pos = command.find_first_of(' ');
    std::string first_part_command = command.substr(0, pos);
    std::string rest_part_command = command.substr(pos + 1);

    switch (commands[first_part_command]) {
        case UPLOAD: {
            if(first_part_command != rest_part_command) {
                return Command::upload(rest_part_command);
            } else {
                std::cout << "Ошибка ввода команды" << std::endl;
                return -1;
            }
            break;
        }
        case DOWNLOAD: {
            if(first_part_command != rest_part_command) {
                return Command::download(rest_part_command);
            } else {
                std::cout << "Ошибка ввода команды" << std::endl;
                return -1;
            }
            break;
        }
        case EXIT: {
            return 1;
            break;
        }
        default: {
            std::cout << "Команды не существует" << std::endl;
            return -1;
            break;
        }
    }

//    if (first_part_command == "chmod") {
//        if(first_part_command != rest_part_command) {
//            return Command::work_with_chmod(rest_part_command);
//        } else {
//            std::cout << "Ошибка ввода команды" << std::endl;
//            return -1;
//        }
//    }
//    if (first_part_command == "mkdir") {
//        if(first_part_command != rest_part_command) {
//            return Command::work_with_directory(rest_part_command);
//        } else {
//            std::cout << "Ошибка ввода команды" << std::endl;
//            return -1;
//        }
//    }
//    if (first_part_command == "login") {
//        if(first_part_command != rest_part_command) {
//            return user.login(rest_part_command);
//        } else {
//            std::cout << "Ошибка ввода команды" << std::endl;
//            return -1;
//        }
//    }
//    if (first_part_command == "logout") {
//        return user.logout();
//    }
//    if (first_part_command == "registration") {
//        if(first_part_command != rest_part_command) {
//            return user.register_user(rest_part_command);
//        } else {
//            std::cout << "Ошибка ввода команды" << std::endl;
//            return -1;
//        }
//    }
//    std::cout << "Команды не существует" << std::endl;
//    return -1;
}

int Client::validation_command(const std::string& command) {
    if (command.empty()) {
        return 2;
    }
    return role_command(command);
}

void Client::run() {

    commands["upload"] = UPLOAD;
    commands["download"] = DOWNLOAD;
    commands["exit"] = EXIT;


    std::string command;
    while (true) {
        print();
        std::getline (std::cin, command);
        int role = validation_command(command);
        if (role == 1) {
            break;
        }
        if (role == 2) {
            continue;
        }
        if (role == 0) {
            std::cout << "SUCСESS" << std::endl;
        } else {
            std::cout << "ERROR" << std::endl;
        }
    }
    std::cout << "good buy" << std::endl;
}

