//
// Created by Yut Fut on 10.12.2021.
//

#include "http_request.h"

void crate_body(const std::string &file_name, std::string &message) {
    char c;
    std::ifstream in(file_name, std::ios::binary);

    if (!in.is_open()) {
        std::cout << "ошибка открытия файла\n";
    }
    while (in.get(c)) {
        message.push_back(c);
    }

    in.close();
    if (in.is_open()) {
        std::cout << "ошибка закрытия файла\n";
    }
}

std::string HTTPRequest::create_message(const std::string &email,
                           const std::string &password,
                           const std::string &jwt,
                           const std::string &current_directory,
                           const std::string &work_directory,
                           const std::string &key,
                           const std::string &file_name,
                           const std::string &command) {
    std::string body, message;

    if (command == "upload") {
        crate_body(file_name, body);
    }

    message = "POST / HTTP/1.0\r\n"
              "email: " + email + "\r\n"
              "password: " + password + "\r\n"
              "jwt: " + jwt + "\r\n"
              "current_directory: " + current_directory + "\r\n"
              "work_directory: " + work_directory + "\r\n"
              "key: " + key + "\r\n"
              "command: " + command + "\r\n"
              "filename: " + file_name + "\r\n"
              "content-length: " + std::to_string(body.size()) + "\r\n\r\n";

    message.append(body);
    message.append("\r\n");
    return message;
}
