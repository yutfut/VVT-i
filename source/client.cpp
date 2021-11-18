//
// Created by Yut Fut on 18.11.2021.
//

#include "../include/client.h"

__attribute__((unused)) void print() {
    std::cout << ">>>\t";
}

int login_on_server() {
    return 0;
}

__attribute__((unused)) int send_on_server() {
    return 0;
}

int valid_email(const std::string& email) {
    if (email.empty()) {
        return -1;
    }
    return 0;
}

int valid_password(const std::string& password) {
    if (password.empty()) {
        return -1;
    }
    return 0;
}

int login(const std::string& command) {
    if (!authorize) {
        size_t pos = command.find_first_of(' ');
        std::string email = command.substr(0, pos);
        if (valid_email(email)) {
            return -1;
        }
        std::string password = command.substr(pos + 1);
        if (valid_password(password)) {
            return -1;
        }
        authorize = true;
        return login_on_server();
    } else {
        std::cout << "Вы уже вошли в систему" << std::endl;
        return 0;
    }
}

int logout() {
    if (authorize) {
        authorize = false;
    } else {
        std::cout << "Вы еще не вошли в систему" << std::endl;
    }

    return 0;
}

int search_file(const std::string& command) {
    size_t pos = command.find_first_of(' ');
    std::string first_part_command = command.substr(0, pos);
    if (valid_email(first_part_command)) {
        return -1;
    }
    std::string rest_part_command = command.substr(pos + 1);
    if (rest_part_command.empty()) {
        std::cout << "Ошибка ввода команды" << std::endl;
        return 1;
    }
    return 0;
}

int valid_chmod(const std::string& command) {
    if (command.empty()) {
        return 1;
    }
    return 0;
}

int parse_chmod(const std::string& command) {
    if (valid_chmod(command)) {
        std::cout << "Ошибка ввода команды" << std::endl;
        return 1;
    }
    return send_on_server();
}

int role_command(const std::string& command) {
    size_t pos = command.find_first_of(' ');
    std::string first_part_command = command.substr(0, pos);
    std::string rest_part_command = command.substr(pos + 1);
    if (first_part_command == "upload") {
        return search_file(rest_part_command);
    }
    if (first_part_command == "chmod") {
        return parse_chmod(rest_part_command);
    }
    if (first_part_command == "exit") {
        return 1;
    }
    if (first_part_command == "login") {
        return login(rest_part_command);
    }
    if (first_part_command == "logout") {
        return logout();
    }
    std::cout << "Команды не существует" << std::endl;
    return -1;
}

int validation_command(const std::string& command) {
    if (command.empty()) {
        std::cout << "Ошибка ввода команды" << std::endl;
        return -1;
    }
    return role_command(command);
}

void get_command() {
    std::string command;
    while (true) {
        print();
        std::getline (std::cin, command);
        int role = validation_command(command);
        if (role == 1) {
            break;
        }
        if (role == 0) {
            role = send_on_server();
        }
        if (role == 0) {
            std::cout << "SUCСESS" << std::endl;
        } else {
            std::cout << "ERROR" << std::endl;
        }
    }
    std::cout << "good buy" << std::endl;
}

