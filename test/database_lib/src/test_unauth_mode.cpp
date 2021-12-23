#include "../include/configurations.h"

TEST(DATA_BASE_TEST, WORK_WITH_TMP_FILES) {

    check_and_create_db();

    DataBase db(VVTI_CONF);
    db.init();

    std::time_t t = std::time(0);   // get time now
    std::tm *now = std::localtime(&t);
    std::string curr_date;
    curr_date += std::to_string(now->tm_year + 1900) + "-" + std::to_string(now->tm_mon + 1)
                 + "-" + std::to_string(now->tm_mday);

    auto unauth_user_file_1 = db.not_auth_mode.add_file("first_file", "password_1");
    auto unauth_user_file_2 = db.not_auth_mode.add_file("second_file", "");

    EXPECT_EQ(db.not_auth_mode.get_upload_file_date(unauth_user_file_1.uuid, "qwertyui").filename, "");
    EXPECT_EQ(db.not_auth_mode.get_upload_file_date(unauth_user_file_1.uuid, "password_1").upload_date, curr_date);
    EXPECT_EQ(db.not_auth_mode.get_upload_file_date(unauth_user_file_2.uuid, "type1").uuid, "");
    EXPECT_EQ(db.not_auth_mode.get_upload_file_date(unauth_user_file_2.uuid, "").upload_date, curr_date);

    db.not_auth_mode.delete_files_by_date(curr_date);
    EXPECT_EQ(db.not_auth_mode.get_upload_file_date(unauth_user_file_1.uuid, "password_1").uuid, "");
}