//
// Created by Yut Fut on 18.11.2021.
//

#include "client.h"

Client::Client(){
    authorize = false;
}

Client::~Client(){}

int Client::valid_email(const std::string& email) {
    if (email.empty()) {
        return -1;
    }
    return 0;
}

int Client::valid_password(const std::string& password) {
    if (password.empty()) {
        return -1;
    }
    return 0;
}

int Client::valid_chmod(const std::string& chmod) {
    if (chmod.empty()) {
        return -1;
    }
    return 0;
}

int Client::valid_directory(const std::string& directory) {
    if (directory.empty()) {
        return -1;
    }
    return 0;
}

int Client::login_on_server() {
    return 0;
}

int Client::registration(const std::string& command) {
    size_t pos = command.find_first_of(' ');
    std::string email = command.substr(0, pos);
    if (valid_email(email)) {
        return -1;
    }
    std::string password = command.substr(pos + 1);
    if (valid_password(password)) {
        return -1;
    }
    return login_on_server();
}

int Client::login(const std::string& command) {
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

int Client::logout() {
    if (authorize) {
        authorize = false;
    } else {
        std::cout << "Вы еще не вошли в систему" << std::endl;
    }

    return login_on_server();
}

void Client::print() {
    std::cout << ">>>\t";
}

void Client::print_data() {
    std::cout << "data" << std::endl;
}

int Client::get_from_server() {
    return 0;
}

int Client::send_on_server() {
    return get_from_server();
}

int Client::search_file(const std::string& file_name) {
    if (file_name.empty()) {
        return -1;
    }
    return 0;
}

int Client::work_with_file(const std::string& command) {
    size_t pos = command.find_first_of(' ');
    std::string first_part_command = command.substr(0, pos);
    if (valid_email(first_part_command)) {
        return -1;
    }
    std::string rest_part_command = command.substr(pos + 1);
    if (search_file(rest_part_command)) {
        return -1;
    }
    return send_on_server();
}

int Client::work_with_chmod(const std::string& command) {
    if (valid_chmod(command)) {
        std::cout << "Ошибка ввода команды" << std::endl;
        return -1;
    }
    return send_on_server();
}

int Client::work_with_directory(const std::string& command) {
    if (valid_directory(command)) {
        std::cout << "Ошибка ввода команды" << std::endl;
        return -1;
    }
    return send_on_server();
}

int Client::role_command(const std::string& command) {
    size_t pos = command.find_first_of(' ');
    std::string first_part_command = command.substr(0, pos);
    std::string rest_part_command = command.substr(pos + 1);
    if (first_part_command == "upload") {
        return work_with_file(rest_part_command);
    }
    if (first_part_command == "chmod") {
        return work_with_chmod(rest_part_command);
    }
    if (first_part_command == "mkdir") {
        return work_with_directory(rest_part_command);
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
    if (first_part_command == "registration") {
        return registration(rest_part_command);
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

