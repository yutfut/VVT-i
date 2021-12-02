//
// Created by Yut Fut on 18.11.2021.
//

#ifndef VVT_I_VALIDATION_H
#define VVT_I_VALIDATION_H

#include <iostream>
#include <string>
#include <regex>

class Validator {
public:
    static int validate_email(const std::string& email);

    static int validate_password(const std::string& password);

    static int validate_key(const std::string& key);

    static int validate_chmod(const std::string& chmod);

    static int validate_directory(const std::string& directory);
};

#endif //VVT_I_VALIDATION_H
