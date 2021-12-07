#include "data_base.h"


DataBase::DataBase(const std::string &user, 
                    const std::string &password,
                    const std::string &port, 
                    const std::string &host, 
                    const std::string &dbname) : 
    connection(DataBaseConnection(user, password, port, host, dbname)),
    transaction(new pqxx::nontransaction(*connection.get_connection(), "transaction")),
    not_auth_mode(NotAuthMode(transaction)), reg_auth(RegAuth(transaction)),
    auth_mode(AuthMode(transaction)), group_mode(GroupMode(transaction)) {}


int DataBase::init() {
    try {
        transaction->exec("CREATE TABLE IF NOT EXISTS Users(id SERIAL PRIMARY KEY, username TEXT NOT NULL, "
                     "email TEXT NOT NULL, password TEXT NOT NULL);");

        transaction->exec("CREATE TABLE IF NOT EXISTS Person_dir(user_id SERIAL REFERENCES Users(id), "
                     "dir_storage_path TEXT NOT NULL);");

        transaction->exec("CREATE TABLE IF NOT EXISTS Groups(id SERIAL PRIMARY KEY, dir_storage_path TEXT UNIQUE);");

        transaction->exec("CREATE TABLE IF NOT EXISTS Group_dir("
                        "user_id SERIAL REFERENCES Users(id), group_id SERIAL REFERENCES Groups(id), group_name TEXT, modifiers TEXT);");


        transaction->exec("CREATE TABLE IF NOT EXISTS Auth_user_files("
                                        "user_id SERIAL REFERENCES Users(id), "
                                        "dir_path TEXT REFERENCES Groups(dir_storage_path));");
        // ADD reference to

        transaction->exec("CREATE EXTENSION IF NOT EXISTS \"uuid-ossp\";");

        transaction->exec("CREATE TABLE IF NOT EXISTS Unauth_user_files("
                                          "uuid UUID PRIMARY KEY DEFAULT uuid_generate_v4(), user_filename TEXT, password TEXT, "
                                          "upload_date DATE DEFAULT CURRENT_DATE);");

    } catch(const pqxx::sql_error& e) {
        throw(e.what());
        return -1;
    }

    return 0;
}
