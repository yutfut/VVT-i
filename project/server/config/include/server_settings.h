#pragma once

#include <string>
#include <vector>
#include <algorithm>
#include <regex>

class ServerSettings {
public:
    ServerSettings() = default;

    ~ServerSettings() = default;

    static const std::vector<std::string> valid_properties;

    int get_number_of_property(const std::string& property);

    void set_property(int number_of_property, std::string value);

    int get_port();

    std::string get_servername();

private:
    int port = 80;

    std::string servername = "127.0.0.1";

    typedef enum {
        LISTEN_NUMBER,
        SERVERNAME_NUMBER
    } numbers_of_properties;
};
