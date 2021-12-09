#pragma once

#include "library_list.h"

class NotAuthMode {
public:

    NotAuthMode(pqxx::nontransaction *transaction);

    unauth_file_data_t add_unauth_user_file(const std::string &user_filename,
                                            const std::string &option_password);       // return unique id-code of file (success) or ""
    int delete_certain_file(std::string uuid);
    int delete_files_by_date(const std::string &upload_date);                       // return uuid (success or not)
    unauth_file_data_t get_upload_file_date(const std::string &file_uuid, const std::string &option_password);             // return date or ""

private:

    pqxx::nontransaction *transaction;
};