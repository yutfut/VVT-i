#include "db_reg_auth.h"


bool RegAuth::is_email_free(const std::string &email) {
    std::string str_query;
    str_query = "SELECT email FROM User WHERE email = " + email + ";";
    pqxx::result res = transaction->exec(str_query);
    if (res.empty()) {
        return true;
    }
    return false;
}

int RegAuth::try_register(const std::string &name, const std::string &email,
                 const std::string &password) {
    if (is_email_free(email)) {
        std::string str_query;
        str_query = "INSERT INTO User (username, email, password) VALUES (" + name + ", " + email + ", " + password + ");";
        pqxx::result res = transaction->exec(str_query);
        return 0;
    }
    return -1;
}

int RegAuth::try_auth(const std::string &email, const std::string &password) {
    std::string str_query;
    str_query = "SELECT * FROM User WHERE email = "+ email + " AND password = " + password + ";";
    pqxx::result res = transaction->exec(str_query);
    if (res.empty()) {
        return -1;
    }

    int id = 0;

    return int(id);
}

int RegAuth::get_id_auth_user(const std::string &email) {
    std::string str_query;
    str_query = "SELECT * FROM User WHERE email = " + email + ";";
    pqxx::result res = transaction->exec(str_query);

    if (res.empty()) {
        return -1;
    }

    int id = 0;

    return id;
}

std::string RegAuth::get_email(int user_id) {
    std::string str_query;
    str_query = "SELECT * FROM User WHERE user_id = " + std::to_string(user_id) + ";";
    pqxx::result res = transaction->exec(str_query);
    if (res.empty()) {
        return "";
    }

    std::string res_str = res[0][0].as<std::string>();

    return res_str;
}
