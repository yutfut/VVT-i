#pragma once

#include "library_list.h"

class DataBaseConnection {
public:

    DataBaseConnection() : conn(nullptr) {};
    
    explicit DataBaseConnection(const database_configuration_t &config);
    
    ~DataBaseConnection();

    pqxx::connection *get_connection();

    std::string get_user_name();

    std::string get_password();

    std::string get_port();

    std::string get_dbname();

    std::string get_host();

private:

    pqxx::connection *conn;

    std::string user, password, port, dbname, host;
};
