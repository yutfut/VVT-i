#ifndef PROJECTNAME_CONFIGURATIONS_H
#define PROJECTNAME_CONFIGURATIONS_H

#include <gtest/gtest.h>
#include <ctime>
#include "data_base.h"
#include "library_list.h"
#include "database_connection.h"
#include "db_not_auth_mode.h"
#include "db_reg_auth.h"
#include "db_single_auth_mode.h"


const database_configuration_t VVTI_CONF = {.user= "postgres",
        .password = "postgres",
        .host = "localhost",
        .port = "5432",
        .dbname = "vvti"};

const database_configuration_t EMPTY_HOST_CONF = {.user= "postgres",
        .password = "postgres",
        .host = "",
        .port = "5432",
        .dbname = "vvti"};

const database_configuration_t NOT_EXIST_DBNAME_CONF = {.user= "postgres",
        .password = "postgres",
        .host = "localhost",
        .port = "5432",
        .dbname = "vvti1"};


static void check_and_create_db() {
    std::string str_query = "postgresql://postgres:postgres@localhost:5432/postgres";
    pqxx::connection conn(str_query.c_str());

    pqxx::nontransaction query(conn);
    query.exec("DROP DATABASE IF EXISTS vvti;");
    query.exec("CREATE DATABASE vvti;");
    query.commit();

    conn.disconnect();
}


#endif //PROJECTNAME_CONFIGURATIONS_H
