#include "data_base.h"


///---DataBase/Tables---///


DataBase::DataBase(const std::string &user, const std::string &password,
                   const std::string &port, const std::string &host, const std::string &dbname) {
    connection = new DataBaseConnection(user, password, port, host, dbname);
    transaction = new pqxx::nontransaction(*connection->get_connection(), "transaction");
}

DataBase::~DataBase() {
    delete transaction;
}

void DataBase::exec_command(const std::string &command) {
        transaction->exec(command);
}

int DataBase::init() {
    try {
        std::string str_query;
        pqxx::result res;

        exec_command("CREATE TABLE IF NOT EXISTS Users(id SERIAL PRIMARY KEY, username TEXT NOT NULL, "
                     "email TEXT NOT NULL, password TEXT NOT NULL);");

        exec_command("CREATE TABLE IF NOT EXISTS Person_dir(user_id SERIAL REFERENCES Users(id), "
                     "dir_storage_path TEXT NOT NULL);");

        exec_command("CREATE TABLE IF NOT EXISTS Groups(id SERIAL PRIMARY KEY, dir_storage_path TEXT UNIQUE);");

        exec_command("CREATE TABLE IF NOT EXISTS Group_dir("
                        "user_id SERIAL REFERENCES Users(id), group_id SERIAL REFERENCES Groups(id), group_name TEXT, modifiers TEXT);");


        exec_command("CREATE TABLE IF NOT EXISTS Auth_user_files("
                                        "user_id SERIAL REFERENCES Users(id), "
                                        "dir_path TEXT REFERENCES Groups(dir_storage_path));");
        // ADD reference to

        exec_command("CREATE EXTENSION IF NOT EXISTS \"uuid-ossp\";");

        exec_command("CREATE TABLE IF NOT EXISTS Unauth_user_files("
                                          "uuid UUID PRIMARY KEY DEFAULT uuid_generate_v4(), user_filename TEXT, password TEXT, "
                                          "upload_date DATE DEFAULT CURRENT_DATE);");

    } catch(const pqxx::sql_error& e) {
        std::cout << "problem: "<< e.what() << std::endl;
        return -1;
    }

    return 0;
}


///---Work in tmp mode---///


unauth_file_data_t DataBase::add_unauth_user_file(const std::string &user_filename,
                                                  const std::string &option_password) {
    unauth_file_data_t c;
    std::string str_query;
    str_query = "INSERT INTO Unauth_user_files(user_filename, password) VALUES('"
                     + user_filename + "', '" + option_password + "') RETURNING uuid, upload_date;";

    pqxx::result res = transaction->exec(str_query);
    unauth_file_data_t add_file_result = {.uuid = res[0][0].as<std::string>(),
                                       .filename = user_filename,
                                       .upload_data = res[0][1].as<std::string>()};

    return add_file_result;
}

int DataBase::delete_unauth_user_files(const std::string &upload_date) {
    std::string str_query;
    str_query = "DELETE FROM Unauth_user_files WHERE upload_date = '" + upload_date + "';";
    pqxx::result res = transaction->exec(str_query);

    return 0;
}

bool DataBase::has_access_on_unauth_user_file(const std::string &file_uuid,
                                              const std::string &option_password) {
    std::string str_query;
    str_query = "SELECT * FROM Unauth_user_files WHERE uuid::text = '" + file_uuid + "' AND password = '" + option_password + "';";
    pqxx::result res = transaction->exec(str_query);

    if (res.empty()) {
        return false;
    }

    return true;
}


///---Reg/auth---///


bool DataBase::is_email_free(const std::string &email) {
    std::string str_query;
    str_query = "SELECT email FROM User WHERE email = " + email + ";";
    pqxx::result res = transaction->exec(str_query);
    if (res.empty()) {
        return true;
    }
    return false;
}

int DataBase::try_register(const std::string &name, const std::string &email,
                 const std::string &password) {
    if (is_email_free(email)) {
        std::string str_query;
        str_query = "INSERT INTO User (username, email, password) VALUES (" + name + ", " + email + ", " + password + ");";
        pqxx::result res = transaction->exec(str_query);
        return 0;
    }
    return -1;
}

int DataBase::try_auth(const std::string &email, const std::string &password) {
    std::string str_query;
    str_query = "SELECT * FROM User WHERE email = "+ email + " AND password = " + password + ";";
    pqxx::result res = transaction->exec(str_query);
    if (res.empty()) {
        return -1;
    }

    int id = 0;

    return int(id);
}

int DataBase::get_id_auth_user(const std::string &email) {
    std::string str_query;
    str_query = "SELECT * FROM User WHERE email = " + email + ";";
    pqxx::result res = transaction->exec(str_query);

    if (res.empty()) {
        return -1;
    }

    int id = 0;

    return id;
}

std::string DataBase::get_email(int user_id) {
    std::string str_query;
    str_query = "SELECT * FROM User WHERE user_id = " + std::to_string(user_id) + ";";
    pqxx::result res = transaction->exec(str_query);
    if (res.empty()) {
        return "";
    }

    std::string res_str = res[0][0].as<std::string>();

    return res_str;
}


///---Work with regular files---///


int DataBase::add_auth_user_file(int user_id, const std::string &dir_path,
                      const std::string &filename) {
    return 0;
}

int DataBase::delete_auth_user_file(int user_id, const std::string &dir_path,
                        const std::string &filename) {
    return 0;
}

int DataBase::change_file_name(int user_id, const std::string &dir_path, const std::string
&old_filename, const std::string &new_filename) {
    return 0;
}

std::vector<std::string> DataBase::get_list_files_in_dir(
        int user_id, const std::string &curr_dir_path) {
    return {};
}


///---Access in group mode---///


int DataBase::create_group(int user_id, const std::string &group_name) {
    return 0;
}

bool DataBase::has_access_on_action(int user_id, int group_id, action_t action) {
    return true;
}

int DataBase::delete_group(int user_id, int group_id) {
    return 0;
}

int DataBase::add_group_member(int user_id, const std::string &group_dir_path,
                     int id_member_to_add) {
    return 0;
}

int DataBase::delete_group_member(int group_id, int user_id,
                        int id_member_to_del) {
    return 0;
}

bool DataBase::is_user_already_in_group(int group_id, int user_id) {
    return true;
}

std::vector<std::pair<std::string, std::string>>
                DataBase::list_users_in_group(int group_id) {
    return {};
}

int DataBase::set_chmod(modifiers_t modes) {
    return 0;
}

modifiers_t DataBase::get_access_modifiers(int group_id) {
    modifiers_t c;
    return c;
}
