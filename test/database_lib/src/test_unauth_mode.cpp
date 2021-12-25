#include "../include/configurations.h"
#include "../include/service_module.h"


TEST(UNAUTH_MODE, WORK_WITH_TMP_FILES) {

    check_and_create_db();

    DataBase db(VALID_CONF);
    db.init();

    std::time_t t = std::time(0);
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


TEST(UNAUTH_MODE, ADD_FILES) {
    check_and_create_db();

    DataBase db(VALID_CONF);
    db.init();

    auto unauth_user_file_1 = db.not_auth_mode.add_file("first_file", "password_1");
    auto unauth_user_file_2 = db.not_auth_mode.add_file("second_file", "password_2");

    EXPECT_EQ(unauth_user_file_1.filename, "first_file");
    EXPECT_EQ(unauth_user_file_1.upload_date, get_current_date());

    EXPECT_EQ(unauth_user_file_2.filename, "second_file");
    EXPECT_EQ(unauth_user_file_2.upload_date, get_current_date());
}


TEST(UNAUTH_MODE, VALID_AND_NOT_VALID_ACCESS_DATA) {
    check_and_create_db();

    DataBase db(VALID_CONF);
    db.init();

    auto unauth_user_file_3 = db.not_auth_mode.add_file("third_file", "password_1");
    auto unauth_user_file_4 = db.not_auth_mode.add_file("fourth_file", "");

    EXPECT_EQ(db.not_auth_mode.get_upload_file_date(unauth_user_file_3.uuid, "qwertyui").filename, "");
    EXPECT_EQ(db.not_auth_mode.get_upload_file_date(unauth_user_file_3.uuid, "password_1").upload_date, get_current_date());
    EXPECT_EQ(db.not_auth_mode.get_upload_file_date(unauth_user_file_4.uuid, "type1").uuid, "");
    EXPECT_EQ(db.not_auth_mode.get_upload_file_date(unauth_user_file_4.uuid, "").upload_date, get_current_date());
}


TEST(UNAUTH_MODE, DELETE_CERTAIN_FILE) {
    check_and_create_db();

    DataBase db(VALID_CONF);
    db.init();

    auto unauth_user_file_5 = db.not_auth_mode.add_file("fieth", "password_1");
    EXPECT_EQ(db.not_auth_mode.get_upload_file_date(unauth_user_file_5.uuid, "password_1").upload_date, get_current_date());

    db.not_auth_mode.delete_file(unauth_user_file_5.uuid);
    EXPECT_EQ(db.not_auth_mode.get_upload_file_date(unauth_user_file_5.uuid, "password_1").upload_date, "");
}


TEST(UNAUTH_MODE, DELETE_FILES_BY_DATE) {
    check_and_create_db();

    DataBase db(VALID_CONF);
    db.init();

    auto unauth_user_file_1 = db.not_auth_mode.add_file("1.txt", "");
    auto unauth_user_file_2 = db.not_auth_mode.add_file("2.txt", "");
    auto unauth_user_file_3 = db.not_auth_mode.add_file("3.txt", "");
    auto unauth_user_file_4 = db.not_auth_mode.add_file("4.txt", "");

    db.not_auth_mode.delete_files_by_date(get_current_date());

    EXPECT_EQ(db.not_auth_mode.get_upload_file_date(unauth_user_file_1.uuid, "").uuid, "");
    EXPECT_EQ(db.not_auth_mode.get_upload_file_date(unauth_user_file_2.uuid, "").uuid, "");
    EXPECT_EQ(db.not_auth_mode.get_upload_file_date(unauth_user_file_3.uuid, "").uuid, "");
    EXPECT_EQ(db.not_auth_mode.get_upload_file_date(unauth_user_file_4.uuid, "").uuid, "");
}
