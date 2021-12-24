#include "../include/configurations.h"
#include "../include/service_module.h"

TEST(DATA_BASE_TEST, WORK_WITH_REGULAR_FILES) {

    check_and_create_db();

    DataBase db(VALID_CONF);
    db.init();

    std::string curr_date = get_current_date();

    auto id = db.reg_auth.try_register("torrent@gmail.com", "new_pass");

    auto def_path = std::to_string(id);

    db.single_auth_mode.create_directory(id, def_path, "first_dir");
    db.single_auth_mode.create_directory(id, def_path, "first_dir/second_dir");

    db.single_auth_mode.add_file(id, def_path + "/first_dir", "1.txt");
    db.single_auth_mode.add_file(id, def_path + "/first_dir", "2.txt");
    db.single_auth_mode.add_file(id, def_path + "/first_dir/second_dir", "2.txt");
    db.single_auth_mode.add_file(id, def_path + "/first_dir", "3.txt");

    db.single_auth_mode.change_filename(1, def_path + "/first_dir", "2.txt", "4.txt");

    std::string files_in_dir = fmt::format("-{0}  {2}  {1}\n-{0}  {3}  {1}\n-{0}  {4}  {1}\n", BASE_ACCESS_LVL,
                                           curr_date, "1.txt", "3.txt", "4.txt");

    std::string dirs_in_dir = fmt::format("d{0}  {2}  {1}\n", BASE_ACCESS_LVL, curr_date, "second_dir");

    EXPECT_EQ(db.single_auth_mode.get_list_files_in_dir(1, def_path + "/first_dir"), files_in_dir + dirs_in_dir);

}
