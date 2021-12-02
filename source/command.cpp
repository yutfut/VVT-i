//
// Created by Yut Fut on 18.11.2021.
//

#include "command.h"


int Command::search_file(const std::string& file_name) {
    if (file_name.empty()) {
        return -1;
    }
    return 0;
}

int Command::work_with_file(const std::string& command) {
    size_t pos = command.find_first_of(' ');
    std::string first_part_command = command.substr(0, pos);
    if (Validator::validate_email(first_part_command)) {
        return -1;
    }
    std::string rest_part_command = command.substr(pos + 1);
    if (first_part_command != rest_part_command) {
        if (search_file(rest_part_command)) {
            return -1;
        }
    } else {
        std::cout << "Ошибка ввода команды" << std::endl;
        return -1;
    }

    std::string password, password_1, y_or_n;

    std::cout << "Хотите задать пароль для файла? y/n:\t";
    std::getline (std::cin, y_or_n);
    if (y_or_n.empty() || y_or_n == "y") {
        while (true) {
            std::cout << "Введите пароль: ";
            std::getline (std::cin, password);
            std::cout << "Введите пароль повторно: ";
            std::getline (std::cin, password_1);
            if (password != password_1 || Validator::validate_password(password)) {
                std::cout << "Пароли не совпадают попробуйте еще раз\n";
            } else {
                break;
            }
        }

    }
    return HTTPRequest::send();
}

int Command::download(const std::string& command) {
    std::string key = command, password;
    if (Validator::validate_key(key)) {
        return -1;
    }
    std::cout << "Введите пароль или нажмите enter если его не указывали:\t";
    while (true) {
        std::getline (std::cin, password);
        if (password.empty() || !Validator::validate_password(password)) {
            break;

        } else {
            std::cout << "Попробуйте еще раз: ";
        }
    }
    return HTTPRequest::send();
}

int Command::work_with_chmod(const std::string& command) {
    if (Validator::validate_chmod(command)) {
        std::cout << "Ошибка ввода команды" << std::endl;
        return -1;
    }
    return HTTPRequest::send();
}

int Command::work_with_directory(const std::string& command) {
    if (Validator::validate_directory(command)) {
        std::cout << "Ошибка ввода команды" << std::endl;
        return -1;
    }
    return HTTPRequest::send();
}