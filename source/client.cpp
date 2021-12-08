//
// Created by Yut Fut on 18.11.2021.
//

#include "client.h"

Client::Client(){}

Client::~Client(){}

void Client::print() {
    std::cout << ">>>\t";
}

int Client::role_command(const std::string& command) {
    size_t pos = command.find_first_of(' ');
    std::string first_part_command = command.substr(0, pos);
    std::string rest_part_command = command.substr(pos + 1);

    switch (commands[first_part_command]) {
        case UPLOAD: {
            if(first_part_command != rest_part_command) {
                return Command::upload(rest_part_command);
            }
            std::cout << "Ошибка ввода команды" << std::endl;
            return -1;
        }
        case DOWNLOAD: {
            if(first_part_command != rest_part_command) {
                return Command::download(rest_part_command);
            }
            std::cout << "Ошибка ввода команды" << std::endl;
            return -1;
        }
        case EXIT: {
            return 1;
        }
        default: {
            std::cout << "Команды не существует" << std::endl;
            return -1;
        }
    }
}

void Client::run() {
    commands["upload"] = UPLOAD;
    commands["download"] = DOWNLOAD;
    commands["exit"] = EXIT;

    std::string command;
    while (true) {
        print();
        std::getline (std::cin, command);
        if (command.empty()) {
            continue;
        }
        int role = role_command(command);
        if (role == 1) {
            break;
        }
        if (role == 0) {
            std::cout << "SUCСESS" << std::endl;
        } else {
            std::cout << "ERROR" << std::endl;
        }
    }
    std::cout << "good buy" << std::endl;
}

