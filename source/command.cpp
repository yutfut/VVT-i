//
// Created by Yut Fut on 18.11.2021.
//

#include "command.h"
#include <cstring>

std::string input_password() {
    std::string password, password_1;
    while (true) {
        password = getpass("Введите пароль: ");
        password_1 = getpass("Введите пароль повторно: ");
        if (password != password_1 || Validator::validate_password(password)) {
            std::cout << "Пароли не совпадают попробуйте еще раз\n";
        } else {
            return password;
        }
    }
}

std::string check_password() {
    std::string y_or_n;
    std::cout << "Хотите задать пароль для файла? y/n:\t";
    std::getline (std::cin, y_or_n);

    if (y_or_n.empty() || y_or_n == "y") {
        return input_password();
    }
    return "";
}

//FILE Command::search_file(const std::string& file_name) {
//    if (file_name.empty()) {
//        return -1;
//    }
//    return 0;
//}

int Command::work_with_file(const std::string& command) {
    size_t pos = command.find_first_of(' ');
    std::string email = command.substr(0, pos);
    if (Validator::validate_email(email)) {
        return -1;
    }

    FILE *fp;
    std::string file_name = command.substr(pos + 1);
    if (email != file_name) {
//        if (search_file(file_name)) {
//            return -1;
//        }
//        if((fp=fopen(file_name.c_str(), "rb+")) == NULL) {
//        if((fp=fopen(std::basic_string<char *>(file_name), "r")) == NULL) {
//            std::cout << "Cannot open file.\n";
//            return -1;
//        }
    } else {
        std::cout << "Ошибка ввода команды" << std::endl;
        return -1;
    }

    std::string password = check_password();
    return HTTPRequest::send(file_name);
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
    return HTTPRequest::send("");
}

int Command::work_with_chmod(const std::string& command) {
    if (Validator::validate_chmod(command)) {
        std::cout << "Ошибка ввода команды" << std::endl;
        return -1;
    }
    return HTTPRequest::send("");
}

int Command::work_with_directory(const std::string& command) {
    if (Validator::validate_directory(command)) {
        std::cout << "Ошибка ввода команды" << std::endl;
        return -1;
    }
    return HTTPRequest::send("");
}