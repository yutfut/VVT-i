//
// Created by Yut Fut on 18.11.2021.
//

#ifndef VVTI_CLIENT_CLIENT_H
#define VVTI_CLIENT_CLIENT_H

#include <iostream>
#include <string>
#include <vector>

class Client {
private:
    bool authorize;
    int id;
    std::string jwt;
    std::string email;
    std::vector<std::string> *data;
public:
    Client();

    ~Client();

    int valid_email(const std::string& email);

    int valid_password(const std::string& password);

    int valid_chmod(const std::string& chmod);

    int valid_directory(const std::string& command);

    int login_on_server();

    int registration(const std::string& command);

    int login(const std::string& command);

    int logout();

    void print();

    void print_data();

    int get_from_server();

    int send_on_server();

    int search_file(const std::string& file_name);

    int work_with_file(const std::string& command);

    int work_with_chmod(const std::string& command);

    int work_with_directory(const std::string& command);

    int role_command(const std::string& command);

    int validation_command(const std::string& command);

    void get_command();
};

#endif //VVTI_CLIENT_CLIENT_H
