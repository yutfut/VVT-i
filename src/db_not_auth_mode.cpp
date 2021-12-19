#include "db_not_auth_mode.h"


NotAuthMode::NotAuthMode(pqxx::connection *conn) : connection(conn) {}


void NotAuthMode::simple_transaction_exec(std::string sql_request) {
    pqxx::work transaction(*connection);
    
    try {
        transaction.exec(sql_request);
        transaction.commit();
    } catch (const pqxx::sql_error &e) {
        transaction.abort();
        std::cout << e.what() << "\n";
        throw(e.what());
    }
}


unauth_file_data_t NotAuthMode::add_unauth_user_file(const std::string &user_filename,
                                                  const std::string &option_password) {
    pqxx::work transaction(*connection);

    try {
        pqxx::result res = transaction.exec(fmt::format(ADD_UNAUTH_USER_FILE, user_filename, option_password));

        unauth_file_data_t add_file_result = {.uuid = res[0][0].as<std::string>(),
                                              .filename = user_filename,
                                              .upload_date = res[0][1].as<std::string>()};

        transaction.commit();

        return add_file_result;
    } catch (const pqxx::sql_error &e) {
        transaction.abort();
        std::cout << e.what() << "\n";
        throw(e.what());
    }
}


void NotAuthMode::delete_certain_file(std::string uuid) {
    
    simple_transaction_exec(fmt::format(DELETE_UNAUTH_USER_FILE, uuid));
}


void NotAuthMode::delete_files_by_date(const std::string &upload_date) {
    
    simple_transaction_exec(fmt::format(DELETE_FILES_BY_DATE, upload_date));
}


unauth_file_data_t NotAuthMode::get_upload_file_date(const std::string &file_uuid,
                                              const std::string &option_password) {
    pqxx::work transaction(*connection);

    try {
        pqxx::result res = transaction.exec(fmt::format(GET_UPLOAD_DATE, file_uuid, option_password));

        if (res.empty()) {
            return {};
        }

        unauth_file_data_t res_struct = {.uuid = file_uuid, .filename = res[0][0].as<std::string>(),
                .upload_date = res[0][1].as<std::string>()};

        transaction.commit();

        return res_struct;
    } catch (const pqxx::sql_error &e) {
        transaction.abort();
        throw(e.what());
    }
}
