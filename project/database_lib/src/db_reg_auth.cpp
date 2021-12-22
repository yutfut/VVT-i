#include "db_reg_auth.h"


RegAuth::RegAuth(pqxx::connection *connection) :
        connection(connection) {}


bool RegAuth::is_email_free(const std::string &email) {

    return trans_check_empty_exec(fmt::format(CHECK_EMAIL_FREE, email), connection);
}


int RegAuth::try_register(const std::string &email,
                          const std::string &password) {

    if (!is_email_free(email)) {

        return -1;
    }

    int id = trans_one_int_value_exec(fmt::format(REGISTER, email, password), connection);
    simple_transaction_exec(fmt::format(COMMAND_MKDIR, id, ROOT_USER_DIR), connection);

    return id;
}


int RegAuth::try_auth(const std::string &email, const std::string &password) {

    return trans_one_int_value_exec(fmt::format(TRY_AUTH, email, password), connection);
}


int RegAuth::get_id_auth_user(const std::string &email) {

    return trans_one_int_value_exec(fmt::format(GET_ID_AUTH_USER, email), connection);
}


std::string RegAuth::get_email(int user_id) {

    return trans_one_string_value_exec(fmt::format(GET_EMAIL_AUTH_USER, user_id), connection);
}
