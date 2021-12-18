#include "db_reg_auth.h"


RegAuth::RegAuth(pqxx::connection *connection) :
                                    connection(connection) {}


void RegAuth::simple_transaction_exec(std::string sql_request) {
    pqxx::work transaction(*connection);
    
    try {
        pqxx::result res = transaction.exec(sql_request);
        transaction.commit();
    } catch (const pqxx::sql_error &e) {
        transaction.abort();
        throw(e.what());
    }
}


bool RegAuth::trans_check_empty_exec(std::string sql_request) {
    pqxx::work transaction(*connection);
    
    try {
        pqxx::result res = transaction.exec(sql_request);
        transaction.commit();

        if (res.empty()) {
            return true;
        }
        
        return false;
    } catch (const pqxx::sql_error &e) {
        transaction.abort();
        throw(e.what());
    }
}


int RegAuth::trans_one_int_value_exec(std::string sql_request) {
    pqxx::work transaction(*connection);
    
    try {
        pqxx::result res = transaction.exec(sql_request);
        transaction.commit();

        if (res.empty()) {
            return -1;
        }

        return res[0][0].as<int>();
    } catch (const pqxx::sql_error &e) {
        transaction.abort();
        throw(e.what());
    }
}


std::string RegAuth::trans_one_string_value_exec(std::string sql_request) {
    pqxx::work transaction(*connection);
    
    try {
        pqxx::result res = transaction.exec(sql_request);
        transaction.commit();

        if (res.empty()) {
            return "";
        }

        return res[0][0].as<std::string>();
    } catch (const pqxx::sql_error &e) {
        transaction.abort();
        throw(e.what());
    }
}


bool RegAuth::is_email_free(const std::string &email) {    
    
    return trans_check_empty_exec(fmt::format(CHECK_EMAIL_FREE, email));
}


int RegAuth::try_register(const std::string &name, const std::string &email,
                 const std::string &password) {
    if (is_email_free(email)) {

            simple_transaction_exec(fmt::format(REGISTER, name, email, password));

            // TODO: add creation user default root directory

        return 0;
    }

    return -1;
}


int RegAuth::try_auth(const std::string &email, const std::string &password) {
    
    return trans_one_int_value_exec(fmt::format(TRY_AUTH, email, password));
}


int RegAuth::get_id_auth_user(const std::string &email) {
    
    return trans_one_int_value_exec(fmt::format(GET_ID_AUTH_USER, email));
}


std::string RegAuth::get_email(int user_id) {
    
    return trans_one_string_value_exec(fmt::format(GET_EMAIL_AUTH_USER, user_id));
}
