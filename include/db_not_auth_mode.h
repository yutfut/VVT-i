#pragma once

#include "library_list.h"

class NotAuthMode {
    
public:

    NotAuthMode(pqxx::connection *conn);

    unauth_file_data_t add_unauth_user_file(const std::string &user_filename,
                                            const std::string &option_password);
    int delete_certain_file(std::string uuid);
    int delete_files_by_date(const std::string &upload_date);
    unauth_file_data_t get_upload_file_date(const std::string &file_uuid, const std::string &option_password);

private:

    pqxx::connection *connection;
};
