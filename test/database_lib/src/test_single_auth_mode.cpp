#include "../include/configurations.h"
#include "../include/service_module.h"


TEST(SINGLE_AUTH_MODE, VALID_DATA_TO_REGISTER) {
    check_and_create_db();

    DataBase db(VALID_CONF);
    db.init();

    std::string curr_date = get_current_date();

    EXPECT_EQ(db.reg_auth.try_register("torrent@gmail.com", "new_pass"), 1);
}


TEST(SINGLE_AUTH_MODE, SAMPLE_EMAILS) {
    check_and_create_db();

    DataBase db(VALID_CONF);
    db.init();

    std::string curr_date = get_current_date();

    db.reg_auth.try_register("sample_email@gmail.com", "new_pass");
    EXPECT_EQ(db.reg_auth.try_register("sample_email@gmail.com", "new_pass"), -1);
}


TEST(SINGLE_AUTH_MODE, ADD_FILE) {
    check_and_create_db();

    DataBase db(VALID_CONF);
    db.init();

    auto id = db.reg_auth.try_register("new_email@gmail.com", "new_pass");

    EXPECT_EQ(db.single_auth_mode.is_filename_free(id, "/", "1.txt"), true);
    EXPECT_EQ(db.single_auth_mode.add_file(id, "/", "1.txt"), true);

    EXPECT_EQ(db.single_auth_mode.is_filename_free(id, "/", "1.txt"), false);
}


TEST(SINGLE_AUTH_MODE, CREATE_DIR) {
    check_and_create_db();

    DataBase db(VALID_CONF);
    db.init();

    auto id = db.reg_auth.try_register("the_newest_email@gmail.com", "the_newest_pass");

    EXPECT_EQ(db.single_auth_mode.is_dir_name_free(id, "/first_dir"), true);
    EXPECT_EQ(db.single_auth_mode.create_directory(id, "/first_dir"), true);

    EXPECT_EQ(db.single_auth_mode.is_dir_name_free(id, "/first_dir"), false);
}


TEST(SINGLE_AUTH_MODE, CHANGE_FILENAME) {
    check_and_create_db();

    DataBase db(VALID_CONF);
    db.init();

    auto id = db.reg_auth.try_register("test@gmail.com", "test_pass");

    db.single_auth_mode.add_file(id, "/", "1.txt");

    EXPECT_EQ(db.single_auth_mode.change_filename(id, "/", "1.txt", "2.txt"), true);
    EXPECT_EQ(db.single_auth_mode.is_filename_free(id, "/", "2.txt"), false);
}


TEST(SINGLE_AUTH_MODE, CHANGE_FILENAME_FILE_ALREADY_EXISTS) {
    check_and_create_db();

    DataBase db(VALID_CONF);
    db.init();

    auto id = db.reg_auth.try_register("technopark@gmail.com", "test_pass");

    db.single_auth_mode.add_file(id, "/", "1.txt");
    db.single_auth_mode.add_file(id, "/", "2.txt");

    EXPECT_EQ(db.single_auth_mode.change_filename(id, "/", "1.txt", "2.txt"), false);
    EXPECT_EQ(db.single_auth_mode.is_filename_free(id, "/", "2.txt"), false);
}


TEST(SINGLE_AUTH_MODE, DELETE_FILE) {
    check_and_create_db();

    DataBase db(VALID_CONF);
    db.init();

    auto id = db.reg_auth.try_register("new_car_kia_rio@gmail.com", "new_pass");
    db.single_auth_mode.add_file(id, "/", "1.txt");
    EXPECT_EQ(db.single_auth_mode.delete_file(id, "/", "2.txt"), false);
    EXPECT_EQ(db.single_auth_mode.delete_file(id, "/", "1.txt"), true);
    EXPECT_EQ(db.single_auth_mode.is_filename_free(id, "/", "1.txt"), true);
}


TEST(SINGLE_AUTH_MODE, RMDIR_NOT_EXIST_DIR) {
    check_and_create_db();

    DataBase db(VALID_CONF);
    db.init();

    auto id = db.reg_auth.try_register("new_car_kia_k9@gmail.com", "new_pass");
    db.single_auth_mode.rmdir(id, "/first/second");
}


TEST(SINGLE_AUTH_MODE, LS_AND_RMDIR_COMMANDS) {
    check_and_create_db();

    DataBase db(VALID_CONF);
    db.init();

    std::string curr_date = get_current_date();

    auto id = db.reg_auth.try_register("new_car_sorento_kia@gmail.com", "new_pass");

    db.single_auth_mode.create_directory(id, "/first_dir");
    db.single_auth_mode.create_directory(id, "/first_dir/second_dir");

    db.single_auth_mode.add_file(id, ROOT_USER_DIR + "first_dir", "1.txt");
    db.single_auth_mode.add_file(id, ROOT_USER_DIR + "first_dir", "2.txt");
    db.single_auth_mode.add_file(id, ROOT_USER_DIR + "first_dir/second_dir", "2.txt");
    db.single_auth_mode.add_file(id, ROOT_USER_DIR + "first_dir", "3.txt");


    std::string files_in_dir = fmt::format(LS_FILES_PATTERN, BASE_ACCESS_LVL, "2.txt", curr_date);
    std::string dirs_in_dir;
    EXPECT_EQ(db.single_auth_mode.get_list_files_in_dir(id, "/first_dir/second_dir"), files_in_dir + dirs_in_dir);


    files_in_dir = fmt::format("-{0}  {2}  {1}\n-{0}  {3}  {1}\n-{0}  {4}  {1}\n", BASE_ACCESS_LVL,
                                           curr_date, "1.txt", "2.txt", "3.txt");
    dirs_in_dir = fmt::format(LS_DIRS_PATTERN, BASE_ACCESS_LVL, "second_dir", curr_date);
    EXPECT_EQ(db.single_auth_mode.get_list_files_in_dir(id, "/first_dir"), files_in_dir + dirs_in_dir);


    EXPECT_EQ(db.single_auth_mode.rmdir(id, "/first_dir"), true);
    EXPECT_EQ(db.single_auth_mode.get_list_files_in_dir(id, "/"), EMPTY_ROOT_DIR_COUT);
}
