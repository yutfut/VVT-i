//
// Created by Yut Fut on 18.11.2021.
//

#include "user.h"

User::User(){
    authorize = true;
}

User::~User(){}

int User::register_user(const std::string& command) {
    std::string email = command;
    if (Validator::validate_email(email)) {
        return -1;
    }

    std::string password = confirm_input_password();

    std::string message = HTTPRequest::create_message(email, password, "", "", "", "", "register");

    std::string http_response = HTTPBase::send(message);
    if (http_response == "ошибка соединения\n") {
        return -1;
    }
    return HTTPResponse::parser(http_response);
}

int User::login(const std::string& command) {
    if (authorize) {
        std::cout << "Вы уже вошли в систему" << std::endl;
        return 0;
    }
    size_t pos = command.find_first_of(' ');
    std::string email = command.substr(0, pos);
    if (Validator::validate_email(email)) {
        return -1;
    }

    std::string password = single_input_password();

    std::string message = HTTPRequest::create_message(email, password, "", "", "", "", "login");

    std::string http_response = HTTPBase::send(message);
    if (http_response == "ошибка соединения\n") {
        return -1;
    }

    if (HTTPResponse::parser(http_response) == 0) {
        authorize = true;
        size_t pos = http_response.find_first_of(' ');
        jwt = command.substr(0, pos);
        current_directory = command.substr(pos + 1);
        return 0;
    }
    return -1;
}

int User::logout() {
    if (!authorize) {
        std::cout << "Вы еще не вошли в систему" << std::endl;
        return 0;
    }
    authorize = false;

    std::string message = HTTPRequest::create_message("", "", jwt,"", "", "", "logout");

    std::string http_response = HTTPBase::send(message);
    if (http_response == "ошибка соединения\n") {
        return -1;
    }
    return HTTPResponse::parser(http_response);
}