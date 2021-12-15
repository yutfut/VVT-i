//
// Created by Yut Fut on 18.11.2021.
//

#include "command.h"

std::string Command::input_password() {
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

std::string Command::check_password_upload() {
    std::string y_or_n;
    std::cout << "Хотите задать пароль для файла? y/n:\t";
    std::getline (std::cin, y_or_n);

    if (y_or_n == "y") {
        return input_password();
    }
    return "";
}

std::string Command::check_password_download() {
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

int Command::upload(const std::string& command) {
    size_t pos = command.find_first_of(' ');
    std::string email = command.substr(0, pos);
    if (Validator::validate_email(email)) {
        return -1;
    }

    std::string password = Command::check_password_upload();

    std::string file_name = command.substr(pos + 1);
    std::string message;
    if (email != file_name) {
        message = HTTPRequest::create_message(email, password, "", file_name, "upload");
    } else {
        std::cout << "Ошибка ввода команды" << std::endl;
        return -1;
    }

    std::string http_response = HTTPBase::send(message);
    if (http_response == "ошибка соединения\n") {
        return -1;
    }
    return HTTPResponse::parser(http_response);
}

int Command::download(const std::string& command) {
    std::string key = command, password;
    if (Validator::validate_key(key)) {
        return -1;
    }
    password = Command::check_password_download();

    std::string message = HTTPRequest::create_message("", password, key, "", "download");

    std::string http_response = HTTPBase::send(message);

    return HTTPResponse::parser(http_response);
}
