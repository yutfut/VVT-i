#include "db_single_auth_mode.h"

#define FILENAME_FREE 1


SingleAuthMode::SingleAuthMode(pqxx::connection *connection) :
        connection(connection) {}


bool SingleAuthMode::is_dir_name_free(int user_id, const std::string &dir_path) {

    return trans_check_empty_exec(fmt::format(CHECK_DIR_NAME_FREE, user_id, dir_path), connection);
}


bool SingleAuthMode::is_filename_free(int user_id, const std::string &dir_path, const std::string &filename) {

    return trans_check_empty_exec(fmt::format(CHECK_FILENAME_FREE, user_id, dir_path, filename), connection);
}


int SingleAuthMode::add_file(int user_id, const std::string &dir_path,
                             const std::string &filename) {

    if (is_filename_free(user_id, dir_path, filename) != FILENAME_FREE) {
        return -1;
    }

    simple_transaction_exec(fmt::format(ADD_AUTH_USER_FILE, user_id, dir_path, filename), connection);

    return 0;
}


int SingleAuthMode::delete_file(int user_id, const std::string &dir_path,
                                const std::string &filename) {

    if (is_filename_free(user_id, dir_path, filename) == FILENAME_FREE) {
        return -1;
    }

    simple_transaction_exec(fmt::format(DELETE_AUTH_USER_FILE, user_id, dir_path, filename), connection);

    return 0;
}


int SingleAuthMode::change_filename(int user_id, const std::string &dir_path, const std::string
&old_filename, const std::string &new_filename) {

    if (is_filename_free(user_id, dir_path, old_filename) != FILENAME_FREE) {
        if (is_filename_free(user_id, dir_path, new_filename) == FILENAME_FREE) {
            simple_transaction_exec(fmt::format(CHANGE_FILENAME, new_filename, user_id, dir_path, old_filename),
                                    connection);
            return 0;
        }
    }

    return -1;
}


int SingleAuthMode::create_directory(int user_id, const std::string &dir_path, const std::string &dir_name) {
    auto separator = "";

    if (dir_path != ROOT_USER_DIR) {
        separator = "/";
    }

    if (!is_dir_name_free(user_id, dir_path + separator + dir_name)) {
        return -1;
    }

    simple_transaction_exec(fmt::format(COMMAND_MKDIR, user_id, dir_path + separator + dir_name), connection);

    return 0;
}


int SingleAuthMode::rmdir(int user_id, const std::string &dir_path, const std::string &dir_name) {
    auto separator = "";

    if (dir_path != ROOT_USER_DIR) {
        separator = "/";
    }

    if (is_dir_name_free(user_id, dir_path + separator + dir_name)) {
        return -1;
    }

    simple_transaction_exec(fmt::format(DELETE_ALL_FILES_IN_DIR, user_id, dir_path + separator + dir_name), connection);
    simple_transaction_exec(fmt::format(COMMAND_RMDIR, user_id, dir_path + separator + dir_name), connection);

    return 0;
}


std::string SingleAuthMode::get_list_files_in_dir(int user_id, const std::string &curr_dir_path) {

    std::string regex_search_dirs = fmt::format(REGEX_SEARCH_INCLUDE_DIRS, curr_dir_path);

    return trans_ls_exec(fmt::format(COMMAND_LS_FILES, user_id, curr_dir_path),
                         fmt::format(COMMAND_LS_DIRS, user_id, regex_search_dirs), connection);
}
