#include "data_base.h"


DataBase::DataBase(const std::string &user, 
                    const std::string &password,
                    const std::string &port, 
                    const std::string &host, 
                    const std::string &dbname) : 
    connection(DataBaseConnection(user, password, port, host, dbname)),
    not_auth_mode(NotAuthMode(connection.get_connection())), reg_auth(RegAuth(connection.get_connection())),
    auth_mode(AuthMode(connection.get_connection())), group_mode(GroupMode(connection.get_connection())) {}

int DataBase::init() {
    pqxx::nontransaction transaction(*connection.get_connection(), "transaction");

    try {
        transaction.exec(CREATE_EXTENSION_UUID);

        transaction.exec(CREATE_EXTENSION_PGCRYPTO);

        transaction.exec(CREATE_TABLE_USERS);

        transaction.exec(CREATE_TABLE_PERSON_DIR);

        transaction.exec(CREATE_TABLE_GROUPS);

        transaction.exec(CREATE_TABLE_GROUP_DIR);

        transaction.exec(CREATE_TABLE_AUTH_USER_FILES);

        transaction.exec(CREATE_TABLE_UNAUTH_USER_FILES);

        transaction.commit();
    } catch(const pqxx::sql_error& e) {
        transaction.abort();
        std::cout << e.what() << "\n";
        throw(e.what());
    }

    return 0;
}
