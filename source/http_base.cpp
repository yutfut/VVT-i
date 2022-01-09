//
// Created by Yut Fut on 18.11.2021.
//

#include "http_base.h"

auto max_size = 1000;

const std::string loading_string = "\x1b[32;1m#\x1b[0m";
const std::string loading_status = "\x1b[32;1m\tCOMPLETE\x1b[0m\n";

const char ip_address[] = "127.0.0.1";

void init_socket_address(struct sockaddr_in &server) {
    server.sin_family = AF_INET;
    server.sin_port = htons(host);
    inet_pton(AF_INET, ip_address, &(server.sin_addr));
}

int request(const int &socket, const std::string& msg) {
    int left = msg.size();
    ssize_t sent = 0;

    while (left > 0) {
        sent = ::send(socket, msg.data() + sent, msg.size() - sent, 0);
        if (sent == -1) {
            std::cout << "ошибка соединения\n";
            return -1;
        }
        left -= sent;
    }

    return 0;
}

int request_loading_string(const int &socket, const std::string& msg) {
    int size_message = msg.size();
    double one_percent_part = size_message / 100;
    size_t sent_size = 0;

    one_percent_part > max_size ? sent_size = max_size : sent_size = one_percent_part;

    double one_percent_send_count = 0;
    size_t print_count = 0;
    ssize_t sent = 0;

    while (size_message > 0) {
        sent = ::send(socket, msg.data() + sent_size, sent_size, 0);
        if (sent == -1) {
            std::cout << std::unitbuf << "ошибка соединения\n";
            return -1;
        }
        size_message -= sent_size;
        size_t count = (size_t)(sent_size / one_percent_part);
        if (count == 0) {
            one_percent_send_count += sent_size / one_percent_part;
        }
        if(one_percent_send_count > 1) {
            std::cout << std::unitbuf << loading_string;
            print_count++;
            one_percent_send_count = 0;
            continue;
        }
        print_count += count;
        if (print_count < 100) {
            for (int i = 0; i < count; ++i) {
                std::cout << std::unitbuf << loading_string;
            }
        } else {
            print_count -= count;
        }
    }

    if (print_count < 100) {
        int n = 100 - print_count;
        for (int i = 0; i < n; ++i) {
            std::cout << std::unitbuf << loading_string;
            print_count++;
        }
    }

    std::cout << std::unitbuf << loading_status;
    return 0;
}

std::string response(const int &socket) {
    char last_char;
    std::string line;
    line.reserve(line_size);
    while (read(socket, &last_char, sizeof(char)) == sizeof(char)) {
        line.push_back(last_char);
    }

    if (line.size() == -1 && errno != EAGAIN) {
        return "ошибка соединения\n";
    }
    if (line.empty()) {
        return "ошибка соединения\n";
    }
    if (line.size() == -1) {
        return "ошибка соединения\n";
    }
    return line;
}

std::string HTTPBase::send(const std::string &message, const std::string &status) {
    int client_socket = socket(PF_INET, SOCK_STREAM, 0);
    if (client_socket <= 0) {
        std::cout << "ошибка соединения\n";
        return "ошибка соединения\n";
    }

    struct sockaddr_in server{};
    init_socket_address(server);

    if (connect(client_socket, (struct sockaddr*)&server, sizeof(server)) != 0) {
        close(client_socket);
        return "ошибка соединения\n";
    }

    if (status == std::string {}) {
        if (request(client_socket, message)) {
            return "ошибка соединения\n";
        }
    } else {
        if (request_loading_string(client_socket, message)) {
            return "ошибка соединения\n";
        }
    }


    std::string http_response = response(client_socket);
    if (http_response == "ошибка соединения\n") {
        std::cout << "ошибка соединения\n";
        return "ошибка соединения\n";
    }

    close(client_socket);

    return http_response;
}
