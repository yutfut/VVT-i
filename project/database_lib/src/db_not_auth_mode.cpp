#include "db_not_auth_mode.h"


NotAuthMode::NotAuthMode(pqxx::nontransaction *transaction) :
                                        transaction(transaction) {}

unauth_file_data_t NotAuthMode::add_unauth_user_file(const std::string &user_filename,
                                                  const std::string &option_password) const {
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

int NotAuthMode::delete_unauth_user_files(const std::string &upload_date) const {
    std::string str_query;
    str_query = "DELETE FROM Unauth_user_files WHERE upload_date = '" + upload_date + "';";
    pqxx::result res = transaction->exec(str_query);

    return 0;
}

std::string NotAuthMode::get_upload_file_date(const std::string &file_uuid,
                                              const std::string &option_password) const {
    std::string str_query;
    str_query = "SELECT upload_date FROM Unauth_user_files WHERE uuid::text = '"
            + file_uuid + "' AND password = '" + option_password + "';";
    pqxx::result res = transaction->exec(str_query);

    if (res.empty()) {
        return "";
    }

    return res[0][0].as<std::string>();
}
