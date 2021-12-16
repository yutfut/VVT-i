#include "db_not_auth_mode.h"

std::string ADD_UNAUTH_USER_FILE = "INSERT INTO Unauth_user_files(user_filename, password) VALUES('{0}', '{1}') RETURNING uuid, upload_date;";
std::string DELETE_FILE_BY_DATE = "DELETE FROM Unauth_user_files WHERE upload_date = '{0}';";
std::string GET_UPLOAD_DATE = "SELECT user_filename, upload_date FROM Unauth_user_files WHERE uuid = '{0}' AND password = '{1}';";
std::string DELETE_CERTAIN_UNAUTH_FILE = "DELETE FROM Unauth_user_files WHERE uuid = '{0}';";

NotAuthMode::NotAuthMode(pqxx::connection *conn) : connection(conn) {}


unauth_file_data_t NotAuthMode::add_unauth_user_file(const std::string &user_filename,
                                                  const std::string &option_password) {
    std::string str_query = "INSERT INTO Unauth_user_files(user_filename, password) VALUES('"
                + user_filename + "', '" + option_password + "') RETURNING uuid, upload_date;";

    pqxx::work transaction(*connection);
    pqxx::result res = transaction.exec(str_query);


    unauth_file_data_t add_file_result = {.uuid = res[0][0].as<std::string>(),
                                       .filename = user_filename,
                                       .upload_date = res[0][1].as<std::string>()};
    transaction.commit();

    return add_file_result;
}


int NotAuthMode::delete_files_by_date(const std::string &upload_date) {
    std::string str_query = "DELETE FROM Unauth_user_files WHERE upload_date = '" + upload_date + "';";

    pqxx::work transaction(*connection);
    pqxx::result res = transaction.exec(str_query);

    transaction.commit();

    return 0;
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
        return {};
    }
}


int NotAuthMode::delete_certain_file(std::string uuid) {
    std::string str_query = "DELETE FROM Unauth_user_files WHERE uuid = '" + uuid + "';";

    pqxx::work transaction(*connection);
    pqxx::result res = transaction.exec(str_query);
    
    transaction.commit();

    return 0;
}
