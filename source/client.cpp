//
// Created by Yut Fut on 18.11.2021.
//

#include "client.h"

Client::Client(){}

Client::~Client(){}

void Client::print() {
    std::cout << ">>>\t";
}

int Client::role_command(const std::string& command) {
    size_t pos = command.find_first_of(' ');
    std::string first_part_command = command.substr(0, pos);
    std::string rest_part_command = command.substr(pos + 1);
    if (first_part_command == "upload") {
        return Command::work_with_file(rest_part_command);
    }
    if (first_part_command == "chmod") {
        return Command::work_with_chmod(rest_part_command);
    }
    if (first_part_command == "mkdir") {
        return Command::work_with_directory(rest_part_command);
    }
    if (first_part_command == "exit") {
        return 1;
    }
    if (first_part_command == "login") {
        return user.login(rest_part_command);
    }
    if (first_part_command == "logout") {
        return user.logout();
    }
    if (first_part_command == "registration") {
        return user.register_user(rest_part_command);
    }
    std::cout << "Команды не существует" << std::endl;
    return -1;
}

int Client::validation_command(const std::string& command) {
    if (command.empty()) {
        std::cout << "Ошибка ввода команды" << std::endl;
        return -1;
    }
    return role_command(command);
}

void Client::get_command() {

    std::string command;
    while (true) {
        print();
        std::getline (std::cin, command);
        int role = validation_command(command);
        if (role == 1) {
            break;
        }
        if (role == 0) {
            std::cout << "SUCСESS" << std::endl;
        } else {
            std::cout << "ERROR" << std::endl;
        }
    }
    std::cout << "good buy" << std::endl;
}

