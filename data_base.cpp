#include "data_base.h"


///---DataBase/Tables---///


int DataBase::init() {
    return 0;
}


///---Reg/auth---///


bool DataBase::is_email_free(const std::string &email) {
    return true;
}

int DataBase::try_register(const std::string &name, const std::string &email,
                 const std::string &password) {
    return 0;
}

int DataBase::try_auth(const std::string &email, const std::string &password) {
    return 0;
}

int DataBase::get_id_auth_user() {
    return 0;
}

std::string DataBase::get_email(int user_id) {
    return "str";
}


///---Work with regular files---///


int DataBase::push_regular_file(int user_id, const std::string &dir_path,
                      const std::string &filename) {
    return 0;
}

int DataBase::delete_regular_file(int user_id, const std::string &dir_path,
                        const std::string &filename) {
    return 0;
}

int DataBase::change_file_name(int user_id, const std::string &dir_path, const std::string
&old_filename, const std::string &new_filename) {
    return 0;
}

std::vector<std::string> DataBase::get_list_files_in_dir(
        int user_id, const std::string &curr_dir_path) {
    return 0;
}


///---Work in tmp mode---///


unauth_file_data_t DataBase::add_unauth_user_file(int user_id,
        const std::string &user_filename, const std::string &option_password) {
    return nullptr;
}

int DataBase::delete_unauth_user_file(const std::string &data_creation) {
    return 0;
}

bool DataBase::has_access_on_unauth_user_file(const std::string &file_id_code,
                                    const std::string &option_password) {
    return true;
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
    return nullptr;
}
