#pragma once

#include "library_list.h"

class DataBaseConnection {
    
public:

    DataBaseConnection(const std::string &user, const std::string &password,
                                const std::string &port, const std::string &host, const std::string &dbname);
    
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
