#ifndef VVT_I_DATA_BASE_CONNECTION_H
#define VVT_I_DATA_BASE_CONNECTION_H

#include <string>
#include <vector>

typedef struct {
    int additional_bit;
    int user;
    int group;
    int other;
} modifiers_t;

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

///---DataBase/Tables---///

    int init();                                     // return 0 if successfully created or 1 if already exists

///---Reg/auth---///

    bool
    check_is_email_free(std::string email);                                 // return 1 if email is free or 0 if not
    int try_register(std::string name, std::string email,
                     std::string password);                                      // return uuid user or -1
    int try_auth(std::string email, std::string password);                       // return uuid user or -1
    int get_id_auth_user();                                                      // return user uuid
    std::string get_email(int user_id);                                          // return user email

///---Work with regular files---///

    int push_regular_file(int user_id, std::string dir_path,
                          std::string filename);                               // return success (0) or already exists (1)
    int delete_regular_file(int user_id, std::string dir_path,
                            std::string filename);                             // return success (0) or not (1)
    int change_file_name(int user_id, std::string dir_path,
                         std::string old_filename, std::string new_filename);  // return file uuid
    std::vector<std::string>
    get_list_files_in_dir(int user_id, std::string curr_dir_path);        // return std::vector of files

///---Work in tmp mode---///

    std::string push_tmp_file(int user_id, std::string user_filename,
                              std::string option_password);                       // return unique id-code of file (success) or ""
    int delete_tmp_files(std::string data_creation);                              // return uuid (success or not)
    bool check_access_on_tmp_file(std::string file_id_code,
                                  std::string option_password);                   // return success (1) or not (0)

///---Access in group mode---///

    int create_group(int user_id,
                     std::string group_name);                        // return group uuid if success or -1 if already exists
    bool check_access(int user_id, int group_id, action_t action);                // return bool (true/false)
    int delete_group(int user_id,
                     int group_id);                                               // return path to group directory
    int add_group_member(int user_id, std::string group_dir_path,
                         int id_member_to_add);                                   // return success (0) or not (1)
    int delete_group_member(int user_id, int group_id,
                            int id_member_to_del);                                // return success (0) or not (1)
    bool check_user_already_in_group(int group_id, int user_id);

    std::vector<std::pair<std::string, std::string>>
    list_users_in_group(int group_id);                                    // return list of users in group
    int set_chmod(modifiers_t modes);                                             // return success (0) or not (1)
    modifiers_t get_access_modifiers(
            int group_id);                               // return modifiers or null if directory is not group


#endif //VVT_I_DATA_BASE_CONNECTION_H
