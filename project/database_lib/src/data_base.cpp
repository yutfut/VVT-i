#include "data_base.h"


DataBase::DataBase(const database_configuration_t &db_conf) :
        connection(DataBaseConnection(db_conf)),
        not_auth_mode(NotAuthMode(connection.get_connection())),
        reg_auth(RegAuth(connection.get_connection())),
        single_auth_mode(SingleAuthMode(connection.get_connection())) {}

int DataBase::init() {
    pqxx::nontransaction transaction(*connection.get_connection(), "transaction");

    try {
        transaction.exec(CREATE_EXTENSION_UUID);

        transaction.exec(CREATE_EXTENSION_PGCRYPTO);

        transaction.exec(CREATE_TABLE_USERS);

        transaction.exec(CREATE_TABLE_PERSON_DIR);

        transaction.exec(CREATE_TABLE_AUTH_USER_FILES);

        transaction.exec(CREATE_TABLE_UNAUTH_USER_FILES);

        transaction.commit();
    } catch (const pqxx::sql_error &e) {
        transaction.abort();
        throw std::string(e.what());
    }

    return 0;
}

DataBaseConnection DataBase::get_connection() {
    return connection;
}
