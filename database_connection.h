#ifndef VVT_I_DATABASE_CONNECTION_H
#define VVT_I_DATABASE_CONNECTION_H

#include <iostream>
#include <string>
#include <pqxx/pqxx>
#include <pqxx/transaction>
#include <pqxx/connection>

#include <pqxx/result.hxx>
#include <pqxx/transaction.hxx>
#include <pqxx/transaction_base>
#include <pqxx/transaction_base.hxx>

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


#endif //VVT_I_DATABASE_CONNECTION_H
