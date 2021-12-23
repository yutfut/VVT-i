// Copyright 2021 Diakonov Andrey

#include <gtest/gtest.h>
#include <ctime>
#include "data_base.h"
#include "library_list.h"
#include "database_connection.h"
#include "db_not_auth_mode.h"
#include "db_reg_auth.h"
#include "db_single_auth_mode.h"
#include "db_group_auth_mode.h"

const database_configuration_t VVTI_CONF = {.user= "postgres",
        .password = "postgres",
        .host = "localhost",
        .port = "5432",
        .dbname = "vvti"};

const database_configuration_t EMPTY_HOST_CONF = {.user= "postgres",
        .password = "postgres",
        .host = "",
        .port = "5432",
        .dbname = "vvti"};

const database_configuration_t NOT_EXIST_DBNAME_CONF = {.user= "postgres",
        .password = "postgres",
        .host = "localhost",
        .port = "5432",
        .dbname = "vvti1"};

///---DataBase/Tables---///

void check_and_create_db() {
    std::string str_query = "postgresql://postgres:postgres@localhost:5432/postgres";
    pqxx::connection conn(str_query.c_str());

    pqxx::nontransaction query(conn);
    query.exec("DROP DATABASE IF EXISTS vvti;");
    query.exec("CREATE DATABASE vvti;");
    query.commit();

    conn.disconnect();
}


TEST(DATA_BASE_TEST, DATA_BASE) {

    check_and_create_db();

    EXPECT_ANY_THROW({
                         DataBase db_1(EMPTY_HOST_CONF);
                     });

    DataBase db_2(VVTI_CONF);
    EXPECT_EQ(db_2.init(), 0);

    EXPECT_ANY_THROW({
                         DataBase db_3(NOT_EXIST_DBNAME_CONF);
                     });
}


///---Work in tmp mode---///


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


///---Reg/auth---///


TEST(DATA_BASE_TEST, CHECK_EMAIL_FREE) {

    check_and_create_db();

    DataBase db(VVTI_CONF);
    db.init();

    db.reg_auth.try_register("fart@gmail.com", "1234");

    EXPECT_EQ(db.reg_auth.is_email_free("fart@gmail.co"), 1);
    EXPECT_EQ(db.reg_auth.is_email_free("fart@gmail.com"), 0);
}

TEST(DATA_BASE_TEST, SAME_EMAILS) {
    DataBase db(VVTI_CONF);

    auto id = db.reg_auth.try_register("fart@gmail.com", "1234");
    EXPECT_EQ(db.reg_auth.try_register("fart@gmail.com", "pass"), -1);
}

TEST(DATA_BASE_TEST, NOT_VALID_DATA_TO_AUTH) {
    DataBase db(VVTI_CONF);

    db.reg_auth.try_register("fart@gmail.com", "1234");
    EXPECT_EQ(db.reg_auth.try_auth("fart@gmail.com", "123"), -1);
    EXPECT_EQ(db.reg_auth.try_auth("fart@gmail.co", "1234"), -1);
}

TEST(DATA_BASE_TEST, VALID_DATA_TO_AUTH) {
    DataBase db(VVTI_CONF);

    EXPECT_EQ(db.reg_auth.try_auth("fart@gmail.com", "1234"), 1);
}

TEST(DATA_BASE_TEST, GET_EMAIL) {
    DataBase db(VVTI_CONF);

    auto id = db.reg_auth.get_id_auth_user("fart@gmail.com");
    EXPECT_EQ(db.reg_auth.get_email(id), "fart@gmail.com");
}


///---Work with regular files---///


TEST(DATA_BASE_TEST, WORK_WITH_REGULAR_FILES) {

    check_and_create_db();

    DataBase db(VVTI_CONF);
    db.init();


    std::time_t t = std::time(0);   // get time now
    std::tm *now = std::localtime(&t);
    std::string curr_date;
    curr_date += std::to_string(now->tm_year + 1900) + "-" + std::to_string(now->tm_mon + 1)
                 + "-" + std::to_string(now->tm_mday);


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


/////---Access in group mode---///


//TEST(DATA_BASE_TEST, Group_auth_mode) {
//    DataBase db(USER, PASSWORD, PORT, HOST, DBNAME);
//    db.reg_auth.try_register("fart@gmail.com", "1234");
//    db.reg_auth.try_register("ivan@mail.ru", "IVAN");
//    db.reg_auth.try_register("sergey@mail.ru", "SERGEY");
//    db.reg_auth.try_register("peter@mail.ru", "PETER");
//
//    db.group_auth_mode.create_group(1, "test_group_1");
//    std::vector<std::pair<std::string, std::string>> list_users;
//
//    list_users.emplace_back("Andrey", "fart@gmail.com");
//    db.group_auth_mode.add_group_member(1, "test_group_1", 2);
//    list_users.emplace_back("Ivan", "ivan@mailru");
//    db.group_auth_mode.add_group_member(1, "test_group_1", 3);
//    list_users.emplace_back("Sergey", "sergey@mailru");
//    db.group_auth_mode.add_group_member(1, "test_group_1", 4);
//    list_users.emplace_back("Peter", "peter@mailru");
//
//    // ASSERT_EQ(db.group_auth_mode.list_users_in_group(1), list_users);
//
//    db.group_auth_mode.delete_group_member(1, 2, 2);
//
//    // EXPECT_EQ(db.group_auth_mode.is_user_already_in_group(1, 2), 0);
//    // EXPECT_EQ(db.group_auth_mode.is_user_already_in_group(1, 3), 1);
//
//    modifiers_t mode = {.additional_bit = 0, .user = 7, .group = 7, .other = 7};
//    db.group_auth_mode.set_chmod(mode);
//    auto set_modes = db.group_auth_mode.get_access_modifiers(1);
//    // EXPECT_EQ(set_modes.additional_bit, mode.additional_bit);
//    // EXPECT_EQ(set_modes.user, mode.user);
//    // EXPECT_EQ(set_modes.group, mode.group);
//    // EXPECT_EQ(set_modes.other, mode.other);
//
//    db.group_auth_mode.has_access_on_action(1, 1, DELETE_GROUP);
//    db.group_auth_mode.delete_group(1, 1);
//
//    EXPECT_EQ(1, 1);
//}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
