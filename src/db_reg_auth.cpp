#include "db_reg_auth.h"


RegAuth::RegAuth(pqxx::connection *connection) :
                                    connection(connection) {}

bool RegAuth::is_email_free(const std::string &email) {
    std::string str_query = "SELECT email FROM Users WHERE email = '" + email + "';";
    
    pqxx::work transaction(*connection);
    pqxx::result res = transaction.exec(str_query);
    transaction.commit();
    
    if (res.empty()) {
        return true;
    }

    return false;
}

int RegAuth::try_register(const std::string &name, const std::string &email,
                 const std::string &password) {
    if (is_email_free(email)) {
        std::string str_query = "INSERT INTO Users (username, email, password) "
            "VALUES ('" + name + "', '" + email + "', '" + password + "');";

        pqxx::work transaction(*connection);
        pqxx::result res = transaction.exec(str_query);
        transaction.commit();

        return 0;
    }

    return -1;
}

int RegAuth::try_auth(const std::string &email, const std::string &password) {
    std::string str_query = "SELECT id FROM Users WHERE email = '" + email + "' AND password = '" + password + "';";
    
    pqxx::work transaction(*connection);
    pqxx::result res = transaction.exec(str_query);
    transaction.commit();
    
    if (res.empty()) {
        return -1;
    }

    return res[0][0].as<int>();
}

int RegAuth::get_id_auth_user(const std::string &email) {
    std::string str_query = "SELECT id FROM Users WHERE email = '" + email + "';";
    
    pqxx::work transaction(*connection);
    pqxx::result res = transaction.exec(str_query);
    transaction.commit();


    if (res.empty()) {
        return -1;
    }

    return res[0][0].as<int>();
}

std::string RegAuth::get_email(int user_id) {
    std::string str_query = "SELECT email FROM Users WHERE user_id = '" + std::to_string(user_id) + "';";
    
    pqxx::work transaction(*connection);
    pqxx::result res = transaction.exec(str_query);
    transaction.commit();
    
    if (res.empty()) {
        return "";
    }

    return res[0][0].as<std::string>();
}
