#include "db_group_auth_mode.h"


GroupAuthMode::GroupAuthMode(pqxx::connection *connection) :
                    connection(connection) {}

int GroupAuthMode::create_group(int user_id, const std::string &group_name) {
    return 0;
}

bool GroupAuthMode::has_access_on_action(int user_id, int group_id, action_t action) {
    return true;
}

int GroupAuthMode::delete_group(int user_id, int group_id) {
    return 0;
}

int GroupAuthMode::add_group_member(int user_id, const std::string &group_dir_path,
                     int id_member_to_add) {
    return 0;
}

int GroupAuthMode::delete_group_member(int group_id, int user_id,
                        int id_member_to_del) {
    return 0;
}

bool GroupAuthMode::is_user_already_in_group(int group_id, int user_id) {
    return true;
}

std::vector<std::pair<std::string, std::string>>
                GroupAuthMode::list_users_in_group(int group_id) {
    return {};
}

int GroupAuthMode::set_chmod(modifiers_t modes) {
    return 0;
}

modifiers_t GroupAuthMode::get_access_modifiers(int group_id) {
    modifiers_t c;
    return c;
}
