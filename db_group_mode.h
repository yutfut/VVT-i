#include <string>

#include "structures.h"

class GroupMode {
public:
    int create_group(int user_id, const std::string &group_name);                 // return group uuid if success or -1 if already exists
    bool has_access_on_action(int user_id, int group_id, action_t action);        // return bool (true/false)
    int delete_group(int user_id, int group_id);                                  // return path to group directory
    int add_group_member(int user_id, const std::string &group_dir_path,
                         int id_member_to_add);                                   // return success (0) or not (1)
    int delete_group_member(int group_id, int user_id,
                            int id_member_to_del);                                // return success (0) or not (1)
    bool is_user_already_in_group(int group_id, int user_id);

    std::vector<std::pair<std::string, std::string>>
            list_users_in_group(int group_id);                                    // return list of users in group
    int set_chmod(modifiers_t modes);                                             // return success (0) or not (1)
    modifiers_t get_access_modifiers(
            int group_id);                               // return modifiers or null if directory is not group
};
