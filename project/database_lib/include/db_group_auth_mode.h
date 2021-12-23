#pragma once

#include "library_list.h"

class GroupAuthMode {

public:

    GroupAuthMode(pqxx::connection *connection);

    int create_group(int user_id, const std::string &group_name);

    bool has_access_on_action(int user_id, int group_id, action_t action);

    int delete_group(int user_id, int group_id);

    int add_group_member(int user_id, const std::string &group_dir_path,
                         int id_member_to_add);

    int delete_group_member(int group_id, int user_id,
                            int id_member_to_del);

    bool is_user_already_in_group(int group_id, int user_id);

    std::vector<std::pair<std::string, std::string>>
    list_users_in_group(int group_id);

    int set_chmod(modifiers_t modes);

    modifiers_t get_access_modifiers(
            int group_id);

private:

    pqxx::connection *connection;
};
