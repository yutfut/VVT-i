#ifndef VVT_I_DATA_BASE_H
#define VVT_I_DATA_BASE_H

#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <pqxx/pqxx>
#include <pqxx/nontransaction>
#include <pqxx/connection>
#include <pqxx/result.hxx>

#include "database_connection.h"
#include "db_not_auth_mode.h"
#include "db_reg_auth.h"
#include "db_auth_mode.h"
#include "db_group_mode.h"
#include "structures.h"


class DataBase {
public:

    DataBase(const std::string &user, const std::string &password,
             const std::string &port, const std::string &host, const std::string &dbname);
             
    ~DataBase();

    int init();

private:

    DataBaseConnection *connection;
    NotAuthMode *not_auth_mode;
    RegAuth *reg_auth;
    AuthMode *auth_mode;
    pqxx::nontransaction *transaction;
};


#endif //VVT_I_DATA_BASE_H
