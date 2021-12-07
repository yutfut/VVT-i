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

std::string check_password_upload() {
    std::string y_or_n;
    std::cout << "Хотите задать пароль для файла? y/n:\t";
    std::getline (std::cin, y_or_n);

    if (y_or_n.empty() || y_or_n == "y") {
        return input_password();
    }
    return "";
}

void crate_body(const std::string &file_name, std::string &message) {
    std::string buff;
    std::ifstream in(file_name);

    if (!in.is_open()) {
        std::cout << "ошибка открытия файла\n";
    }
    while (getline(in, buff)) {
        message.append(buff.append("\n"));
    }

    in.close();
}

std::string create_message(const std::string &email,
                           const std::string &password,
                           const std::string &key,
                           const std::string &file_name,
                           const std::string &command) {
    std::string message, body="";

    if (command == "upload") {
        crate_body(file_name, body);
    }

    message = "email: " + email + "\n"
              "password: " + password + "\n"
              "key: " + key + "\n"
              "status: \n"
              "message: \n"
              "command: " + command + "\n"
              "filename: " + file_name + "\n\n";

    message.append(body);
    return message;
}

int Command::upload(const std::string& command) {
    size_t pos = command.find_first_of(' ');
    std::string email = command.substr(0, pos);
    if (Validator::validate_email(email)) {
        return -1;
    }

    std::string password = check_password_upload();

    std::string file_name = command.substr(pos + 1);
    std::string message;
    if (email != file_name) {
        message = create_message(email, password, "", file_name, "upload");
    } else {
        std::cout << "Ошибка ввода команды" << std::endl;
        return -1;
    }

    return HTTPRequest::send(message);
}

std::string check_password_download() {
    std::string password;
    std::cout << "Введите пароль или нажмите enter если его не указывали:\t";
    while (true) {
        std::getline (std::cin, password);
        if (password.empty()) {
            return "";
        }
        if (!Validator::validate_password(password)) {
            return password;
        } else {
            std::cout << "Попробуйте еще раз: ";
        }
    }
}

int Command::download(const std::string& command) {
    std::string key = command, password;
    if (Validator::validate_key(key)) {
        return -1;
    }
    password = check_password_download();

    std::string message = create_message("", password, key, "", "download");

    return HTTPRequest::send(message);
}

void create_file(const std::string &file_name, std::string &message) {
    std::ofstream out(file_name);

    if (out.is_open()) {
        out << message;
    } else {
        std::cout << "ошибка открытия файла\n";
    }

    out.close();
}

//int Command::work_with_chmod(const std::string& command) {
//    if (Validator::validate_chmod(command)) {
//        std::cout << "Ошибка ввода команды" << std::endl;
//        return -1;
//    }
//    return HTTPRequest::send("", "", "", "");
//}
//
//int Command::work_with_directory(const std::string& command) {
//    if (Validator::validate_directory(command)) {
//        std::cout << "Ошибка ввода команды" << std::endl;
//        return -1;
//    }
//    return HTTPRequest::send("", "", "", "");
//}