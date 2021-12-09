//
// Created by Yut Fut on 18.11.2021.
//

#include "validation.h"

int Validator::validate_email(const std::string& email) {
    std::regex email_regex{ ".+@.+\\..+" };
    if (!email.empty()) {
        bool is_valid = std::regex_search(email, email_regex);
        if (is_valid == 1) {
            return 0;
        } else {
            std::cout << "email не удовлетворяет регулярному выражению\n";
            return -1;
        }
    } else {
        return -1;
    }

}

int Validator::validate_password(const std::string& password) {
    std::regex password_regex{ "^(?=.*?[A-Z])(?=.*?[a-z])(?=.*?[0-9])(?=.*?[#?!@$%^&*-]).{8,}$" };
    if (!password.empty()) {
        bool is_valid = std::regex_search(password, password_regex);
        if (is_valid == 1) {
            return 0;
        } else {
            std::cout << "пароль не удовлетворяет регулярному выражению\n";
            return -1;
        }
    } else {
        return -1;
    }
}

int Validator::validate_key(const std::string& key) {
    if (key.empty()) {
        return -1;
    }
    return 0;
}