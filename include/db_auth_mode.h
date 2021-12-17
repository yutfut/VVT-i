#pragma once

#include "library_list.h"

class AuthMode {

public:

    AuthMode(pqxx::connection *connection);

    int add_auth_user_file(int user_id, const std::string &dir_path,
                          const std::string &filename);

    int delete_auth_user_file(int user_id, const std::string &dir_path,
                            const std::string &filename);

    int change_file_name(int user_id, const std::string &dir_path, const std::string
    &old_filename, const std::string &new_filename);
    
    std::vector<std::string>
    get_list_files_in_dir(int user_id, const std::string &curr_dir_path);

private:

    pqxx::connection *connection;
};
