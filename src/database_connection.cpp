#include "database_connection.h"

DataBaseConnection::DataBaseConnection(const database_configuration_t &config) :
        user(config.user), password(config.password), port(config.port),
        host(config.host), dbname(config.dbname), conn(nullptr) {
    
    if (user.empty() || password.empty() || port.empty() || host.empty() || dbname.empty()) {
        throw std::string("Error :  Check database configuration, not all settings filled\n");
    }
    
    std::string str_query = fmt::format("postgresql://{0}:{1}@{2}:{3}/{4}", user, password, host, port, dbname);

    try {
        conn = new pqxx::connection(str_query);
    } catch (const pqxx::sql_error &e) {
        throw std::string(e.what());
    }

}

DataBaseConnection::~DataBaseConnection() {
    if (conn != nullptr && conn->is_open()) {
        conn->disconnect();
        delete conn;
    }
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
