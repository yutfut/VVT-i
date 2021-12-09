#include "db_not_auth_mode.h"


NotAuthMode::NotAuthMode(pqxx::nontransaction *transaction) :
        transaction(transaction) {}

unauth_file_data_t NotAuthMode::add_unauth_user_file(const std::string &user_filename,
                                                     const std::string &option_password) {
    unauth_file_data_t c;
    std::string str_query;
    str_query = "INSERT INTO Unauth_user_files(user_filename, password) VALUES('"
                + user_filename + "', '" + option_password + "') RETURNING uuid, upload_date;";

    pqxx::result res = transaction->exec(str_query  );
    unauth_file_data_t add_file_result = {.uuid = res[0][0].as<std::string>(),
            .filename = user_filename,
            .upload_data = res[0][1].as<std::string>()};

    return add_file_result;
}

int NotAuthMode::delete_files_by_date(const std::string &upload_date) {
    std::string str_query;
    str_query = "DELETE FROM Unauth_user_files WHERE upload_date = '" + upload_date + "';";
    pqxx::result res = transaction->exec(str_query);

    return 0;
}

unauth_file_data_t NotAuthMode::get_upload_file_date(const std::string &file_uuid,
                                                     const std::string &option_password) {
    std::string str_query;

    try {
        str_query = "SELECT user_filename, upload_date FROM Unauth_user_files WHERE uuid = '"
                    + file_uuid + "' AND password = '" + option_password + "';";

        pqxx::result res = transaction->exec(str_query);

        if (res.empty()) {
            return {};
        }

        unauth_file_data_t res_struct = {.uuid = file_uuid, .filename = res[0][0].as<std::string>(),
                .upload_data = res[0][1].as<std::string>()};

        return res_struct;
    } catch (const pqxx::sql_error &e) {
        return {};
    }
}

int NotAuthMode::delete_certain_file(std::string uuid) {
    std::string str_query;

    str_query = "DELETE FROM Unauth_user_files WHERE uuid = '" + uuid + "';";
    pqxx::result res = transaction->exec(str_query);

    return 0;
}