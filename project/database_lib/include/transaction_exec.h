#pragma once

#include "library_list.h"

class TransactionExec {

protected:

    static void simple_transaction_exec(const std::string &sql_request, pqxx::connection *connection);

    static bool trans_check_empty_exec(const std::string &sql_request, pqxx::connection *connection);

    static int trans_one_int_value_exec(const std::string &sql_request, pqxx::connection *connection);

    static std::string trans_one_string_value_exec(const std::string &sql_request, pqxx::connection *connection);

    std::string trans_ls_exec(const std::string &sql_request_file,
                              const std::string &sql_request_dir, DirType dir_type, pqxx::connection *connection);

private:

    std::string get_name_dir(const std::string &dir_path);

};