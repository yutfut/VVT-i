#pragma once

#include "library_list.h"

class TransactionExec {

protected:
    
    void simple_transaction_exec(std::string sql_request, pqxx::connection *connection);

    bool trans_check_empty_exec(std::string sql_request, pqxx::connection *connection);

    int trans_one_int_value_exec(std::string sql_request, pqxx::connection *connection);
    
    std::string trans_one_string_value_exec(std::string sql_request, pqxx::connection *connection);

};