#pragma once

#include "library_list.h"

class NotAuthMode {
public:

    NotAuthMode(pqxx::nontransaction *transaction);

    unauth_file_data_t add_unauth_user_file(const std::string &user_filename,
                                            const std::string &option_password) const;       // return unique id-code of file (success) or ""
    int delete_unauth_user_files(const std::string &upload_date) const;                       // return uuid (success or not)
    bool has_access_on_unauth_user_file(const std::string &file_uuid,
                                        const std::string &option_password) const;             // return success (1) or not (0)

private:

     pqxx::nontransaction *transaction; //TODO: это поле может быть неконстантным? это безопасно для нескольких процессов?
};
// TODO: сделать методы константными, если это возможно