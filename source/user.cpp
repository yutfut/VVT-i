//
// Created by Yut Fut on 18.11.2021.
//

#include "user.h"

User::User(){
    authorize = false;
}

User::~User(){}

int User::register_user(const std::string& command) {
    size_t pos = command.find_first_of(' ');
    std::string email = command.substr(0, pos);
    if (Validator::validate_email(email)) {
        return -1;
    }
    std::string password = command.substr(pos + 1);
    if (Validator::validate_password(password)) {
        return -1;
    }
    return 0;
}

int User::login(const std::string& command) {
    if (!authorize) {
        size_t pos = command.find_first_of(' ');
        std::string email = command.substr(0, pos);
        if (Validator::validate_email(email)) {
            return -1;
        }
        std::string password = command.substr(pos + 1);
        if (Validator::validate_password(password)) {
            return -1;
        }
        authorize = true;
        return 0;
    } else {
        std::cout << "Вы уже вошли в систему" << std::endl;
        return 0;
    }
}

int User::logout() {
    if (authorize) {
        authorize = false;
    } else {
        std::cout << "Вы еще не вошли в систему" << std::endl;
    }

    return 0;
}