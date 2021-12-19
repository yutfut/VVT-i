#include "transaction_exec.h"


void TransactionExec::simple_transaction_exec(std::string sql_request, pqxx::connection *connection) {
    pqxx::work transaction(*connection);
    
    try {
        transaction.exec(sql_request);
        transaction.commit();
    } catch (const pqxx::sql_error &e) {
        transaction.abort();
        throw(e.what());
    }
}


bool TransactionExec::trans_check_empty_exec(std::string sql_request, pqxx::connection *connection) {
    pqxx::work transaction(*connection);
    
    try {
        pqxx::result res = transaction.exec(sql_request);
        transaction.commit();

        if (res.empty()) {
            return true;
        }
        
        return false;
    } catch (const pqxx::sql_error &e) {
        transaction.abort();
        throw(e.what());
    }
}


int TransactionExec::trans_one_int_value_exec(std::string sql_request, pqxx::connection *connection) {
    pqxx::work transaction(*connection);
    
    try {
        pqxx::result res = transaction.exec(sql_request);
        transaction.commit();

        if (res.empty()) {
            return -1;
        }

        return res[0][0].as<int>();
    } catch (const pqxx::sql_error &e) {
        transaction.abort();
        throw(e.what());
    }
}


std::string TransactionExec::trans_one_string_value_exec(std::string sql_request, pqxx::connection *connection) {
    pqxx::work transaction(*connection);
    
    try {
        pqxx::result res = transaction.exec(sql_request);
        transaction.commit();

        if (res.empty()) {
            return "";
        }

        return res[0][0].as<std::string>();
    } catch (const pqxx::sql_error &e) {
        transaction.abort();
        throw(e.what());
    }
}
