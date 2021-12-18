#include "db_single_auth_mode.h"

#define FILENAME_FREE 1


SingleAuthMode::SingleAuthMode(pqxx::connection *connection) :
                                    connection(connection) {}


void SingleAuthMode::simple_transaction_exec(std::string sql_request) {
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


bool SingleAuthMode::trans_check_empty_exec(std::string sql_request) {
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


int SingleAuthMode::add_auth_user_file(int user_id, const std::string &dir_path,
                      const std::string &filename) {
    // TODO: May be not need
    if (trans_check_empty_exec(fmt::format(CHECK_FILENAME_FREE, user_id, dir_path, filename)) == FILENAME_FREE) {
        simple_transaction_exec(fmt::format(ADD_AUTH_USER_FILE, user_id, dir_path, filename));
        return 0;
    }
    
    return -1;
}


int SingleAuthMode::delete_auth_user_file(int user_id, const std::string &dir_path,
                                                    const std::string &filename) {
                                                        
    if (trans_check_empty_exec(fmt::format(CHECK_FILENAME_FREE, user_id, dir_path, filename)) != FILENAME_FREE) {
        simple_transaction_exec(fmt::format(DELETE_AUTH_USER_FILE, user_id, dir_path, filename));
        return 0;
    }
    
    return -1;
}


int SingleAuthMode::change_filename(int user_id, const std::string &dir_path, const std::string
                                            &old_filename, const std::string &new_filename) {

    // May be not need
    if (trans_check_empty_exec(fmt::format(CHECK_FILENAME_FREE, user_id, dir_path, old_filename)) != FILENAME_FREE) {
        if (trans_check_empty_exec(fmt::format(CHECK_FILENAME_FREE, user_id, dir_path, new_filename)) == FILENAME_FREE) {
            simple_transaction_exec(fmt::format(CHANGE_FILENAME, user_id, dir_path, new_filename));
            return 0;
        }
    }

    return -1;
}


// std::vector<std::string> SingleAuthMode::get_list_files_in_dir(int user_id, const std::string &curr_dir_path) {

//     va_list list_files;

//     return {};
// }
