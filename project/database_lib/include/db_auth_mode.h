#pragma once

#include "library_list.h"

class AuthMode {
public:

    AuthMode(pqxx::nontransaction *trans);

    int add_auth_user_file(int user_id, const std::string &dir_path,
                          const std::string &filename);                               // return success (0) or already exists (1)
    int delete_auth_user_file(int user_id, const std::string &dir_path,
                            const std::string &filename);                             // return success (0) or not (1)
    int change_file_name(int user_id, const std::string &dir_path, const std::string
    &old_filename, const std::string &new_filename);                                  // return file uuid
    std::vector<std::string>
    get_list_files_in_dir(int user_id, const std::string &curr_dir_path);             // return std::vector of files

private:

    pqxx::nontransaction *transaction;
};
