#pragma once

#include <stdarg.h>

#include "library_list.h"

class SingleAuthMode : public TransactionExec {

public:

    SingleAuthMode(pqxx::connection *connection);

    int add_file(int user_id, const std::string &dir_path,
                          const std::string &filename);

    int delete_file(int user_id, const std::string &dir_path,
                            const std::string &filename);

    int create_directory(int user_id, const std::string &dir_path, const std::string &dir_name);

    int change_filename(int user_id, const std::string &dir_path, const std::string
    &old_filename, const std::string &new_filename);

    bool is_dir_name_free(int user_id, const std::string &dir_path);

    bool is_filename_free(int user_id, const std::string &dir_path, const std::string &filename);
    
    std::vector<std::string>
    get_list_files_in_dir(int user_id, const std::string &curr_dir_path);

private:

    pqxx::connection *connection;

};
