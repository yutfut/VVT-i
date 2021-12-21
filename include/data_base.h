#pragma once

#include "library_list.h"
#include "database_connection.h"
#include "db_not_auth_mode.h"
#include "db_reg_auth.h"
#include "db_single_auth_mode.h"
#include "db_group_auth_mode.h"


class DataBase {

private:

    DataBaseConnection connection;

public:

    DataBase() = default;

    DataBase(database_configuration_t &db_conf);

    ~DataBase() = default;

    int init();

    NotAuthMode not_auth_mode;
    RegAuth reg_auth;
    SingleAuthMode single_auth_mode;
    GroupAuthMode group_auth_mode;
};
