//
// Created by Yut Fut on 18.11.2021.
//

#include "command.h"

int upload(const std::string& command, User &user) {
    size_t pos = command.find_first_of(' ');

    std::string email = command.substr(0, pos);

    if (!user.get_authorize()) {
        if (Validator::validate_email(email)) {
            return -1;
        }
    }

    std::string file_name = command.substr(pos + 1);
    file_name = std::filesystem::path(file_name).filename();

    if (!user.get_authorize()) {
        if (email == file_name) {
            std::cout << "Ошибка ввода команды" << std::endl;
            return -1;
        }
    }

    std::string message, actual_file_name;

    if (user.get_authorize()) {
        message = HTTPRequest::create_message(std::string{},
                                    std::string{},
                                    user.get_jwt(),
                                    user.get_current_directory(),
                                    std::string{},
                                    std::string{},
                                    file_name,
                                    "has");
        if (message == "ошибка работы с файлом\n") {
            return -1;
        }

        std::string http_response = HTTPBase::send(message);

        if (http_response == "ошибка соединения\n") {
            return -1;
        }

        if (HTTPResponse::parser(http_response) == 2) {
            std::cout << "Файл с таким названием уже существует\n";
            std::cout << "Хотите его переименовать? y/n:\t";
            std::getline (std::cin, actual_file_name);

            if (!actual_file_name.empty()) {
                file_name = actual_file_name;
            }
        }
    }

    if (!user.get_authorize()) {
        std::string password = check_password_upload();
        message = HTTPRequest::create_message(email,
                                              password,
                                              std::string {},
                                              std::string {},
                                              std::string {},
                                              std::string {},
                                              file_name,
                                              "upload");
    } else {
        message = HTTPRequest::create_message(std::string {},
                                              std::string {},
                                              user.get_jwt(),
                                              user.get_current_directory(),
                                              std::string {},
                                              std::string {},
                                              file_name,
                                              "upload");
    }

    if (message == "ошибка работы с файлом\n") {
        return -1;
    }

    std::string http_response = HTTPBase::send(message);
    if (http_response == "ошибка соединения\n") {
        return -1;
    }
    return HTTPResponse::parser(http_response);
}

int download(const std::string& command, User &user) {
    std::string key = command,
                      message;

    if (!user.get_authorize()) {
        if (Validator::validate_key(key)) {
            return -1;
        }
    }

    if (!user.get_authorize()) {
        std::string password = check_password_download();
        message = HTTPRequest::create_message(std::string{},
                                                          password,
                                                          user.get_jwt(),
                                                          std::string{},
                                                          std::string{},
                                                          key,
                                                          std::string{},
                                                          "download");
    } else {
        message = HTTPRequest::create_message(std::string{},
                                              std::string{},
                                              user.get_jwt(),
                                              user.get_current_directory(),
                                              std::string{},
                                              std::string{},
                                              key,
                                              "download");
    }

    if (message == "ошибка работы с файлом\n") {
        return -1;
    }

    std::string http_response = HTTPBase::send(message);

    return HTTPResponse::parser(http_response);
}

int work_with_directory(const std::string& first_part_command, const std::string& rest_part_command, User &user) {
    if (rest_part_command.empty()) {
        return -1;
    }

    if (first_part_command == "rmdir") {
        std::string y;
        std::cout << "Вы уверены?: ";
        std::getline (std::cin, y);
        if (y != "y") {
            return 0;
        }
    }

    if (user.get_authorize() && first_part_command == "rm") {
        std::string message = HTTPRequest::create_message(std::string {},
                                                          std::string {},
                                                          user.get_jwt(),
                                                          user.get_current_directory(),
                                                          std::string {},
                                                          std::string {},
                                                          rest_part_command,
                                                          first_part_command);

        std::string http_response = HTTPBase::send(message);

        return HTTPResponse::parser(http_response);
    }

    if (user.get_authorize()) {
        std::string message = HTTPRequest::create_message(std::string {},
                                                          std::string {},
                                                          user.get_jwt(),
                                                          user.get_current_directory(),
                                                          rest_part_command,
                                                          std::string {},
                                                          std::string {},
                                                          first_part_command);

        std::string http_response = HTTPBase::send(message);

        if (first_part_command == "cd") {
            if (HTTPResponse::parser(http_response) == 0) {
                user.set_current_directory((user.get_current_directory() / rest_part_command).lexically_normal());
                return 0;
            }
        } else {
            return HTTPResponse::parser(http_response);
        }
        return -1;
    }

    std::cout << "Вы не авторизованы!\n";
    return -1;
}