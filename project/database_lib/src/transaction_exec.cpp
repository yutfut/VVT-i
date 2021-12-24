#include "transaction_exec.h"


void TransactionExec::simple_transaction_exec(const std::string &sql_request, pqxx::connection *connection) {
    pqxx::work transaction(*connection);

    try {
        transaction.exec(sql_request);
        transaction.commit();
    } catch (const pqxx::sql_error &e) {
        transaction.abort();
        std::cout << e.what() << "\n";
        throw std::string(e.what());
    }
}


bool TransactionExec::trans_check_empty_exec(const std::string &sql_request, pqxx::connection *connection) {
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
        throw std::string(e.what());
    }
}


int TransactionExec::trans_one_int_value_exec(const std::string &sql_request, pqxx::connection *connection) {
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
        throw std::string(e.what());
    }
}


std::string TransactionExec::trans_one_string_value_exec(const std::string &sql_request, pqxx::connection *connection) {
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
        throw std::string(e.what());
    }
}


std::string TransactionExec::get_name_dir(const std::string &dir_path) {

    return dir_path.substr(dir_path.find_last_of('/') + 1, dir_path.length());
}


std::string TransactionExec::trans_ls_exec(const std::string &sql_request_file,
                                           const std::string &sql_request_dir, pqxx::connection *connection) {

    pqxx::work transaction(*connection);
    std::string ls_result;

    try {
        pqxx::result res = transaction.exec(sql_request_file);

        for (const auto &re: res) {
            auto fmt_string = fmt::format("-{0}  {1}  {2}\n", BASE_ACCESS_LVL,
                                          re[0].as<std::string>(), re[1].as<std::string>());
            ls_result += fmt_string;
        }

        res = transaction.exec(sql_request_dir);
        transaction.commit();

        for (int i = 0; i < res.size(); ++i) {
            ls_result += fmt::format("d{0}  {1}  {2}\n", BASE_ACCESS_LVL,
                                     get_name_dir(res[i][0].as<std::string>()), res[i][1].as<std::string>());
        }

        return ls_result;

    } catch (const pqxx::sql_error &e) {
        transaction.abort();
        throw std::string(e.what());
    }
}
