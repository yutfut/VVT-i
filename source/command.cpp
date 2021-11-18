//
// Created by Yut Fut on 18.11.2021.
//

#include "command.h"

int Command::search_file(const std::string& file_name) {
    if (file_name.empty()) {
        return -1;
    }
    return 0;
}

int Command::work_with_file(const std::string& command) {
    size_t pos = command.find_first_of(' ');
    std::string first_part_command = command.substr(0, pos);
    if (Validator::validate_email(first_part_command)) {
        return -1;
    }
    std::string rest_part_command = command.substr(pos + 1);
    if (search_file(rest_part_command)) {
        return -1;
    }
    return HTTPRequest::send();
}

int Command::work_with_chmod(const std::string& command) {
    if (Validator::validate_chmod(command)) {
        std::cout << "Ошибка ввода команды" << std::endl;
        return -1;
    }
    return HTTPRequest::send();
}

int Command::work_with_directory(const std::string& command) {
    if (Validator::validate_directory(command)) {
        std::cout << "Ошибка ввода команды" << std::endl;
        return -1;
    }
    return HTTPRequest::send();
}