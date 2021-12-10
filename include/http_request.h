//
// Created by Yut Fut on 10.12.2021.
//

#ifndef VVT_I_HTTP_REQUEST_H
#define VVT_I_HTTP_REQUEST_H

#include <iostream>
#include <string>
#include <fstream>

class HTTPRequest {
public:
    static std::string create_message(const std::string &email,
                               const std::string &password,
                               const std::string &key,
                               const std::string &file_name,
                               const std::string &command);
};

#endif //VVT_I_HTTP_REQUEST_H
