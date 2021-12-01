#pragma once

#include <string>
#include <vector>
#include <algorithm>
#include <regex>

typedef struct {
    std::string user = "postgres";

    std::string password = "postgres";

    std::string host = "localhost";

    int port = 5432;

    std::string dbname = "VVT-i";
} database_t;

class ServerSettings {
public:
    ServerSettings() = default;

    ~ServerSettings() = default;

    static const std::vector<std::string> valid_properties;

    static const std::vector<std::string> database_valid_properties;

    int get_number_of_property(const std::string& property);

    void set_property(int number_of_property, std::string value);

    int get_number_of_database_property(const std::string& property);

    void set_database_property(int number_of_property, std::string value);

    int get_port();

    std::string get_servername();

    database_t get_database();

private:
    int port = 80;

    std::string servername = "127.0.0.1";

    database_t database;

    typedef enum {
        LISTEN_NUMBER,
        SERVERNAME_NUMBER
    } numbers_of_properties;

    typedef enum {
        USER_NUMBER,
        PASSWORD_NUMBER,
        HOST_NUMBER,
        PORT_NUMBER,
        DBNAME_NUMBER
    } numbers_of_database_properties;
};
