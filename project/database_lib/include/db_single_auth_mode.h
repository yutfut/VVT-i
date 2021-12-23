#pragma once

#include "library_list.h"

class SingleAuthMode : public TransactionExec {

public:

    explicit SingleAuthMode(pqxx::connection *connection = nullptr);

    bool add_file(int user_id, const std::string &dir_path,
                  const std::string &filename);

    bool delete_file(int user_id, const std::string &dir_path,
                     const std::string &filename);

    bool create_directory(int user_id, const std::string &dir_path, const std::string &dir_name);

    bool rmdir(int user_id, const std::string &dir_path, const std::string &dir_name);

    bool change_filename(int user_id, const std::string &dir_path, const std::string
    &old_filename, const std::string &new_filename);

    bool is_dir_name_free(int user_id, const std::string &dir_path);

    bool is_filename_free(int user_id, const std::string &dir_path, const std::string &filename);

    std::string get_list_files_in_dir(int user_id, const std::string &curr_dir_path);

private:

    pqxx::connection *connection;

};
