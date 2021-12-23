#pragma once

#include "library_list.h"

class RegAuth : public TransactionExec {

public:

    explicit RegAuth(pqxx::connection *connection = nullptr);

    bool is_email_free(const std::string &email);

    int try_register(const std::string &email,
                     const std::string &password);

    int try_auth(const std::string &email, const std::string &password);

    int get_id_auth_user(const std::string &email);

    std::string get_email(int user_id);

private:

    pqxx::connection *connection;

};
