#include "database_connection.h"

DataBaseConnection::DataBaseConnection(const std::string &user, const std::string &password,
          const std::string &port, const std::string &host, const std::string &dbname) :
          user(user), password(password), port(port), host(host), dbname(dbname) {
    std::string str_query;
    if (user.empty() || password.empty() || port.empty() || host.empty() || dbname.empty()) {

        str_query = "postgresql://postgres:postgres@localhost:5432/postgres";

        try {
            conn = new pqxx::connection(str_query.c_str());
        } catch (const pqxx::sql_error& e) {
            std::cout << "problem: " << e.what() << std::endl;
            return;
        }

        auto transaction = new pqxx::work(*conn, "transaction");

        bool db_already_exist = transaction->exec("SELECT COUNT(*) FROM pg_database WHERE datname ='vvti'")[0][0].as<bool>();
        transaction->commit();

        if (!db_already_exist) {
            pqxx::nontransaction query(*conn);
            query.exec("CREATE DATABASE vvti;");
            query.commit();
        }

        str_query = "postgresql://postgres:postgres@localhost:5432/vvti";

        try {
            delete conn;
            conn = new pqxx::connection(str_query);
        } catch (const pqxx::sql_error& e) {
            std::cout << "problem: " << e.what() << std::endl;
            return;
        }

        delete transaction;
    } else {
        str_query = "user='" + user + "' password='" + password + "' host='" + host +
                    "' port='" + port + "' dbname='" + dbname + "'";

        try {
            conn = new pqxx::connection(str_query);
        } catch (const pqxx::sql_error& e) {
            std::cout << "problem: " << e.what() << std::endl;
            return;
        }
    }
}

DataBaseConnection::~DataBaseConnection() {
    conn->disconnect();
}

pqxx::connection *DataBaseConnection::get_connection() {
    return conn;
}

std::string DataBaseConnection::get_user_name() {
    return user;
}

std::string DataBaseConnection::get_password() {
    return password;
}

std::string DataBaseConnection::get_port() {
    return port;
}

std::string DataBaseConnection::get_dbname() {
    return dbname;
}

std::string DataBaseConnection::get_host() {
    return host;
}
