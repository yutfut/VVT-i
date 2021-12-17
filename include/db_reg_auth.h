#pragma once

#include "library_list.h"

class RegAuth {

public:

    RegAuth(pqxx::connection *connection);

    bool is_email_free(const std::string &email);

    int try_register(const std::string &name, const std::string &email,
                     const std::string &password);

    int try_auth(const std::string &email, const std::string &password);

    int get_id_auth_user(const std::string &email);
    
    std::string get_email(int user_id);

private:

    pqxx::connection *connection;

    void simple_transaction_exec(std::string sql_request);

    bool trans_check_empty_exec(std::string sql_request);

    int trans_one_int_value_exec(std::string sql_request);
    
    std::string trans_one_string_value_exec(std::string sql_request);
};
