#include "db_auth_mode.h"


AuthMode::AuthMode(pqxx::nontransaction *transaction) :
                                    transaction(transaction) {}

int AuthMode::add_auth_user_file(int user_id, const std::string &dir_path,
                      const std::string &filename) {
    return 0;
}

int AuthMode::delete_auth_user_file(int user_id, const std::string &dir_path,
                        const std::string &filename) {
    return 0;
}

int AuthMode::change_file_name(int user_id, const std::string &dir_path, const std::string
&old_filename, const std::string &new_filename) {
    return 0;
}

std::vector<std::string> AuthMode::get_list_files_in_dir(
        int user_id, const std::string &curr_dir_path) {
    return {};
}
