#pragma once

#include "library_list.h"
#include "database_connection.h"
#include "db_not_auth_mode.h"
#include "db_reg_auth.h"
#include "db_auth_mode.h"
#include "db_group_mode.h"


class DataBase {
public:

    DataBase();
    DataBase(const std::string &user, const std::string &password,
             const std::string &port, const std::string &host, const std::string &dbname);
             
    ~DataBase() = default;

    int init();

    NotAuthMode not_auth_mode;
    RegAuth reg_auth;
    AuthMode auth_mode;
    GroupMode group_mode;

private:

    DataBaseConnection connection;
    pqxx::nontransaction *transaction;
};
