#pragma once

#include "library_list.h"

class RegAuth {
public:

    RegAuth(pqxx::nontransaction* transaction);

    bool is_email_free(const std::string &email);                                    // return 1 if email is free or 0 if not
    int try_register(const std::string &name, const std::string &email,
                     const std::string &password);                                   // return uuid user or -1
    int try_auth(const std::string &email, const std::string &password);             // return uuid user or -1
    int get_id_auth_user(const std::string &email);                                                          // return user uuid
    std::string get_email(int user_id);                                              // return user email

private:

    pqxx::nontransaction* transaction;
};
