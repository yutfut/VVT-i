//
// Created by Yut Fut on 18.11.2021.
//

#include "client.h"

Client::Client(){}

Client::~Client(){}

void Client::print(User &user) {
    if (user.get_authorize()) {
        std::cout << ">>> " << std::string(user.get_current_directory()) << "$:\t";
    } else {
        std::cout << ">>>\t";
    }

}

void help() {
    std::cout << "не авторизованный пользователь\n"
                 "\tupload <email> <file name>\n"
                 "\tdownload <key>\n\n"
                 "авторизованный пользователь\n"
                 "\tregister <email> <password> <password>\n"
                 "\tlogin <email> <password>\n"
                 "\tlogout\n"
                 "\tcd <path>\n"
                 "\tls [<path>]\n"
                 "\tmkdir <name>\n"
                 "\trmdir <name>\n"
                 "\trm <file name>\n\n";
}

int Client::role_command(const std::string& command, User &user) {
    size_t pos = command.find_first_of(' ');
    std::string first_part_command = command.substr(0, pos);
    std::string rest_part_command = command.substr(pos + 1);

    switch (commands[first_part_command]) {
        case Commands::UPLOAD: {
            if(first_part_command != rest_part_command) {
                return upload(rest_part_command, user);
            }
            std::cout << "Ошибка ввода команды" << std::endl;
            return -1;
        }
        case Commands::DOWNLOAD: {
            if(first_part_command != rest_part_command) {
                return download(rest_part_command, user);
            }
            std::cout << "Ошибка ввода команды" << std::endl;
            return -1;
        }
        case Commands::REGISTER: {
            if(first_part_command != rest_part_command) {
                return user.register_user(rest_part_command);
            }
            std::cout << "Ошибка ввода команды" << std::endl;
            return -1;
        }
        case Commands::LOGIN: {
            if(first_part_command != rest_part_command) {
                return user.login(rest_part_command);
            }
            std::cout << "Ошибка ввода команды" << std::endl;
            return -1;
        }
        case Commands::LOGOUT: {
            return user.logout();
        }
        case Commands::DIR: {
            if(first_part_command != rest_part_command) {
                return work_with_directory(first_part_command, rest_part_command, user);
            }
            std::cout << "Ошибка ввода команды" << std::endl;
            return -1;
        }
        case Commands::HELP: {
            help();
            return 0;
        }
        case Commands::EXIT: {
            return 1;
        }
        default: {
            std::cout << "Команды не существует" << std::endl;
            return -1;
        }
    }
}

void Client::run() {
    commands[UPLOAD] = Commands::UPLOAD;
    commands[DOWNLOAD] = Commands::DOWNLOAD;
    commands[REGISTER] = Commands::REGISTER;
    commands[LOGIN] = Commands::LOGIN;
    commands[LOGOUT] = Commands::LOGOUT;
    commands[CD] = Commands::DIR;
    commands[LS] = Commands::DIR;
    commands[MKDIR] = Commands::DIR;
    commands[RMDIR] = Commands::DIR;
    commands[EXIT] = Commands::EXIT;
    commands[HELP] = Commands::HELP;

    User user;

    std::cout << "Welcome to VVTi!" << std::endl;
    std::string command;

    std::vector<std::string> history;

    while (true) {
        print(user);

        std::getline (std::cin, command);

        if (command.empty()) {
            continue;
        }

        auto status = (Status) role_command(command, user);
        switch (status){
            case Status::SUCCESS:
                std::cout << "SUCСESS" << std::endl;
                break;
            case Status::GOODBYE:
                std::cout << "GOODBYE" << std::endl;
                return;
            default:
                std::cout << "ERROR" << std::endl;
                break;
        }
    }

}

