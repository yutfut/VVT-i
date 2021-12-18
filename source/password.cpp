//
// Created by Yut Fut on 18.12.2021.
//

#include "password.h"

std::string single_input_password() {
    std::string password;
    while (true) {
        password = getpass("Введите пароль:\t");
        if (!Validator::validate_password(password)) {
            return password;
        }
        std::cout << "Попробуйте еще раз: ";
    }
}

std::string confirm_input_password() {
    std::string password, password_confirm;
    while (true) {
        password = getpass("Введите пароль: ");
        password_confirm = getpass("Введите пароль повторно: ");
        if (password != password_confirm || Validator::validate_password(password)) {
            std::cout << "Пароли не совпадают попробуйте еще раз\n";
        } else {
            return password;
        }
    }
}

std::string check_password_upload() {
    std::string y_or_n;
    std::cout << "Хотите задать пароль для файла? y/n:\t";
    std::getline (std::cin, y_or_n);

    if (y_or_n == "y") {
        return confirm_input_password();
    }
    return {};
}

std::string check_password_download() {
    std::string password;
    while (true) {
        password = getpass("Введите пароль или нажмите enter если его не указывали:\t");
        if (password.empty()) {
            return "";
        }
        if (!Validator::validate_password(password)) {
            return password;
        }
        std::cout << "Попробуйте еще раз: ";
    }
}