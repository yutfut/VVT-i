#ifndef VVT_I_DATA_BASE_H
#define VVT_I_DATA_BASE_H

#include <string>
#include <vector>
#include <ctime>

typedef struct {
    int additional_bit;
    int user;
    int group;
    int other;
} modifiers_t;


typedef struct {
    std::string filename;
    std::string upload_data;
} unauth_file_data_t;


typedef enum {
    DELETE_GROUP,
    ADD_MEMBER,
    DELETE_MEMBER,
    SET_CHMOD_ON_GROUP,
    SET_CHMOD,
    SET_UMASK,
    UPLOAD,
    DOWNLOAD,
    READ,
    WRITE
} action_t;


class DataBase {
public:
///---DataBase/Tables---///

    int init();                                     // return 0 if successfully created or 1 if already exists

///---Reg/auth---///

    bool
    is_email_free(const std::string &email);                                    // return 1 if email is free or 0 if not
    int try_register(const std::string &name, const std::string &email,
                     const std::string &password);                                   // return uuid user or -1
    int try_auth(const std::string &email, const std::string &password);             // return uuid user or -1
    int get_id_auth_user();                                                          // return user uuid
    std::string get_email(int user_id);                                              // return user email

///---Work with regular files---///

    int push_regular_file(int user_id, const std::string &dir_path,
                          const std::string &filename);                               // return success (0) or already exists (1)
    int delete_regular_file(int user_id, const std::string &dir_path,
                            const std::string &filename);                             // return success (0) or not (1)
    int change_file_name(int user_id, const std::string &dir_path, const std::string
    &old_filename, const std::string &new_filename);  // return file uuid
    std::vector<std::string>
    get_list_files_in_dir(int user_id, const std::string &curr_dir_path);             // return std::vector of files

///---Work in tmp mode---///

    unauth_file_data_t add_unauth_user_file(int user_id,
            const std::string &user_filename, const std::string &option_password);       // return unique id-code of file (success) or ""
    int delete_unauth_user_file(const std::string &data_creation);                       // return uuid (success or not)
    bool has_access_on_unauth_user_file(const std::string &file_id_code,
                                        const std::string &option_password);             // return success (1) or not (0)

///---Access in group mode---///

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


#endif //VVT_I_DATA_BASE_H
