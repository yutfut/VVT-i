//
// Created by Yut Fut on 18.11.2021.
//

#ifndef VVTI_CLIENT_CLIENT_H
#define VVTI_CLIENT_CLIENT_H

#include <iostream>

bool authorize = false;

void print();

int login_on_server();

int send_on_server();

int valid_email(const std::string& email);

int valid_password(const std::string& password);

int login(const std::string& command);

int logout();

int search_file(const std::string& command);

int valid_chmod(const std::string& command);

int parse_chmod(const std::string& command);

int role_command(const std::string& command);

int validation_command(const std::string& command);

void get_command();

#endif //VVTI_CLIENT_CLIENT_H
