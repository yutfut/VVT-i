//
// Created by Yut Fut on 10.12.2021.
//

#include "http_request.h"

#include <sstream>
#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/filter/gzip.hpp>

std::string create_body(const std::string &file_name) {
    std::ifstream file(file_name, std::ios::binary);

    if (!file.is_open()) {
        std::cout << "ошибка открытия файла\n";
    }

    std::stringstream comp, decomp;

    boost::iostreams::filtering_streambuf<boost::iostreams::input> in;
    in.push(boost::iostreams::gzip_compressor());
    in.push(file);
    boost::iostreams::copy(in, comp);

    file.close();
    if (file.is_open()) {
        std::cout << "ошибка закрытия файла\n";
    }

    return comp.str();
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
        body = create_body(file_name);
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
