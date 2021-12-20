//
// Created by Yut Fut on 10.12.2021.
//

#include "http_response.h"

void create_file(const std::string &file_name, const std::string &message) {
    std::ofstream out(file_name);

    if (out.is_open()) {
        out << message;
    } else {
        std::cout << "ошибка открытия файла\n";
    }

    out.close();
}

int HTTPResponse::parser(std::string &http) {
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
        if(rest_part_header.empty()) {
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
    if (commands["command"] == "download" && commands["message"] == "OK") {
        create_file(commands["filename"], commands["body"]);
    }

    if (!commands["jwt"].empty()) {
        http = commands["jwt"];
    }

    if (commands["message"] == "OK") {
        return 0;
    }
    return -1;
}