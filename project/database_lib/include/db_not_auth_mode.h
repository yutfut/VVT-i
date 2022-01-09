#pragma once

#include "library_list.h"


class NotAuthMode : public TransactionExec {

public:

    explicit NotAuthMode(pqxx::connection *conn = nullptr);

    unauth_file_data_t add_file(const std::string &user_filename,
                                const std::string &option_password);

    void delete_file(const std::string &uuid);

    void delete_files_by_date(const std::string &upload_date);

    unauth_file_data_t get_upload_file_date(const std::string &file_uuid, const std::string &option_password);

private:

    pqxx::connection *connection;

};
