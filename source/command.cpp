//
// Created by Yut Fut on 18.11.2021.
//

#include "command.h"

std::string input_password() {
    std::string password, password_1;
    while (true) {
        password = getpass("Введите пароль: ");
        password_1 = getpass("Введите пароль повторно: ");
        if (password != password_1 || Validator::validate_password(password)) {
            std::cout << "Пароли не совпадают попробуйте еще раз\n";
        } else {
            return password;
        }
    }
}

std::string check_password_upload() {
    std::string y_or_n;
    std::cout << "Хотите задать пароль для файла? y/n:\t";
    std::getline (std::cin, y_or_n);

    if (y_or_n == "y") {
        return input_password();
    }
    return "";
}

std::string check_password_download() {
    std::string password;
    while (true) {
        password = getpass("Введите пароль или нажмите enter если его не указывали:\t");
        if (password.empty()) {
            return "";
        }
        if (!Validator::validate_password(password)) {
            return password;
        }
        std::cout << "Попробуйте еще раз: ";
    }
}

void create_file(const std::string &file_name, const std::string &message) {
    std::ofstream out(file_name);

    if (out.is_open()) {
        out << message;
    } else {
        std::cout << "ошибка открытия файла\n";
    }

    out.close();
}

int parser(std::string http) {
    std::map<std::string, std::string> commands;

    size_t pos = http.find_first_of('\n');
    http = http.substr(pos + 1);

    while (!http.empty()) {
        pos = http.find_first_of('\r');
        std::string header = http.substr(0, pos);
        http = http.substr(pos + 1);

        size_t pos1 = header.find_first_of(':');
        std::string first_part_header = header.substr(0, pos1);
        std::string rest_part_header = header.substr(pos1 + 1);
        if( rest_part_header.empty()) {
            break;
        }
        rest_part_header.erase(rest_part_header.begin(), rest_part_header.begin() + 1);
        commands[first_part_header] = rest_part_header;

        if (http[0] == '\n' && http[2] == '\n' && http[1] == '\r') {
            if (commands["command"] == "upload") {
                break;
            }
            http.erase(http.begin(), http.begin() + 3);
            commands["body"] = http;
            break;
        }
        http.erase(http.begin(), http.begin() + 1);
    }

    std::cout << std::endl;
    if (commands["command"] == "download" && commands["message"] == "OK") {
        create_file(commands["filename"], commands["body"]);
    }

    if (commands["message"] == "OK") {
        return 0;
    }
    return -1;
}

void crate_body(const std::string &file_name, std::string &message) {
    std::string buff;
    std::ifstream in(file_name);

    if (!in.is_open()) {
        std::cout << "ошибка открытия файла\n";
    }
    while (getline(in, buff)) {
        message.append(buff.append("\r\n"));
    }

    in.close();
}

std::string create_message(const std::string &email,
                           const std::string &password,
                           const std::string &key,
                           const std::string &file_name,
                           const std::string &command) {
    std::string message, body="";

    if (command == "upload") {
        crate_body(file_name, body);
    }

    message = "POST / HTTP/1.0\r\n"
              "email: " + email + "\r\n"
              "password: " + password + "\r\n"
              "key: " + key + "\r\n"
              "status: \r\n"
              "message: \r\n"
              "command: " + command + "\r\n"
              "filename: " + file_name + "\r\n"
              "content-length: " + std::to_string(body.size()) + "\r\n\r\n";

    message.append(body);
    return message;
}

int Command::upload(const std::string& command) {
    size_t pos = command.find_first_of(' ');
    std::string email = command.substr(0, pos);
    if (Validator::validate_email(email)) {
        return -1;
    }

    std::string password = check_password_upload();

    std::string file_name = command.substr(pos + 1);
    std::string message;
    if (email != file_name) {
        message = create_message(email, password, "", file_name, "upload");
    } else {
        std::cout << "Ошибка ввода команды" << std::endl;
        return -1;
    }

    std::string http_response = HTTPRequest::send(message);
    if (http_response == "ошибка соединения\n") {
        return -1;
    }
    return parser(http_response);
}

int Command::download(const std::string& command) {
    std::string key = command, password;
    if (Validator::validate_key(key)) {
        return -1;
    }
    password = check_password_download();

    std::string message = create_message("", password, key, "", "download");

    std::string http_response = HTTPRequest::send(message);

    return parser(http_response);
}
