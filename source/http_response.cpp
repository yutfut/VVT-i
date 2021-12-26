//
// Created by Yut Fut on 10.12.2021.
//

#include "http_response.h"

#include <sstream>
#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/filter/gzip.hpp>

std::string check_exist_file(const std::string& file_name) {
    bool exist = std::filesystem::exists(file_name);
    if (exist) {
        std::cout << "такой файл уже существует";
        std::string yes_or_no, new_file_name;
        std::cout << "хотите его переименовать? y/n:\t";
        std::getline (std::cin, yes_or_no);
        if (yes_or_no == "y") {
            std::cout << "введите новое название: ";
            std::getline (std::cin, new_file_name);
            return new_file_name;
        }
    }
    return file_name;
}

void create_file(const std::string &file_name, const std::string &message) {
    std::string actual_file_name = check_exist_file(file_name);

    std::ofstream out(actual_file_name);

    if (out.is_open()) {
        std::stringstream ss, comp, decomp;
        ss << message;

        boost::iostreams::filtering_streambuf<boost::iostreams::input> inbuf;
        inbuf.push(boost::iostreams::gzip_decompressor());
        inbuf.push(ss);
        boost::iostreams::copy(inbuf, decomp);

        out << decomp.str();
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
            http = http.substr(0, atoi(commands["content-length"].c_str()));
            commands["body"] = http;
            break;
        }
        http.erase(http.begin(), http.begin() + 1);
    }

    if (commands["command"] != "download") {
        std::cout << commands["body"] << std::endl;
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

    if (commands["message"] == "RENAME" && commands["command"] == "has") {
        return 2;
    }

    return -1;
}
