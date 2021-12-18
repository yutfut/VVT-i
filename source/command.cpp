//
// Created by Yut Fut on 18.11.2021.
//

#include "command.h"

int Command::upload(const std::string& command) {
    size_t pos = command.find_first_of(' ');
    std::string email = command.substr(0, pos);
    if (Validator::validate_email(email)) {
        return -1;
    }

    std::string password = check_password_upload();

    std::string file_name = command.substr(pos + 1);

    if (email == file_name) {
        std::cout << "Ошибка ввода команды" << std::endl;
        return -1;
    }

    std::string message = HTTPRequest::create_message(email, password, "", file_name, "upload");

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
    password = check_password_download();

    std::string message = HTTPRequest::create_message("", password, key, "", "download");

    std::string http_response = HTTPBase::send(message);

    return HTTPResponse::parser(http_response);
}
