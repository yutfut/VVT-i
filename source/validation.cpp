//
// Created by Yut Fut on 18.11.2021.
//

#include "validation.h"

int Validator::validate_email(const std::string& email) {
    if (email.empty()) {
        return -1;
    }
    return 0;
}

int Validator::validate_password(const std::string& password) {
    if (password.empty()) {
        return -1;
    }
    return 0;
}

int Validator::validate_chmod(const std::string& chmod) {
    if (chmod.empty()) {
        return -1;
    }
    return 0;
}

int Validator::validate_directory(const std::string& directory) {
    if (directory.empty()) {
        return -1;
    }
    return 0;
}