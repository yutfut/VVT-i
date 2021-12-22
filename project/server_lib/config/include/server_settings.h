#pragma once

#include <string>
#include <array>
#include <algorithm>
#include <regex>

struct database_t {
    std::string user = "postgres";

    std::string password = "postgres";

    std::string host = "localhost";

    int port = 5432;

    std::string dbname = "VVT-i";
};

class ServerSettings {
public:
    ServerSettings() = default;

    ~ServerSettings() = default;

    std::array<std::string, 5> valid_properties = {
        "listen", "servername", "fs_root", "database"
    };

    std::array<std::string, 5> database_valid_properties = {
        "user", "password", "host", "port", "dbname"
    };

    int get_number_of_property(const std::string& property);

    void set_property(int number_of_property, std::string value);

    int get_number_of_database_property(const std::string& property);

    void set_database_property(int number_of_property, std::string value);

    int get_port();

    std::string get_servername();

    std::string get_fs_root();

    database_t get_database();

private:
    void set_port(const std::string& value, int begin, int value_length);

    void set_port_number(const std::string& value, int begin, int value_length);

    int port = 80;

    std::string servername = "127.0.0.1";

    std::string fs_root = "/";

    database_t database;

    enum class numbers_of_properties_t {
        LISTEN_NUMBER,
        SERVERNAME_NUMBER,
        FS_ROOT
    };

    enum class numbers_of_database_properties_t {
        USER_NUMBER,
        PASSWORD_NUMBER,
        HOST_NUMBER,
        PORT_NUMBER,
        DBNAME_NUMBER
    };
};
