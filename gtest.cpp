// Copyright 2021 Diakonov Andrey

#include <gtest/gtest.h>

#include "data_base_connection.h"


///---DataBase/Tables---///


TEST(DATA_BASE_TEST, CREATE_DATA_BASE) {
    EXPECT_EQ(init(), 0);
}

TEST(DATA_BASE_TEST, DOUBLE_CREATE) {
    EXPECT_EQ(init(), 1);
}


///---Reg/auth---///


TEST(DATA_BASE_TEST, CHECK_EMAIL_FREE) {
    EXPECT_EQ(check_is_email_free("fart@gmail.co"), 1);
    EXPECT_EQ(check_is_email_free("fart@gmail.com"), 0);
}

TEST(DATA_BASE_TEST, SAME_EMAILS) {
    try_register("Andrey", "fart@gmail.com", "1234");
    EXPECT_EQ(try_register("Andrey", "fart@gmail.com", "1234"), -1);
}

TEST(DATA_BASE_TEST, NOT_VALID_DATA_TO_AUTH) {
    EXPECT_EQ(try_auth("fart@gmail.com", "123"), -1);
    EXPECT_EQ(try_auth("fart@gmail.co", "1234"), -1);
}

TEST(DATA_BASE_TEST, VALID_DATA_TO_AUTH) {
    EXPECT_EQ(try_auth("fart@gmail.com", "1234"), 1);
}

TEST(DATA_BASE_TEST, GET_EMAIL) {
    get_id_auth_user();
    EXPECT_EQ(get_email(1), "fart@gmail.com");
}


///---Work with regular files---///


TEST(DATA_BASE_TEST, WORK_WITH_REGULAR_FILES) {
    push_regular_file(1, "first_dir", "1.txt");
    push_regular_file(1, "first_dir", "2.txt");
    push_regular_file(1, "first_dir/second_dir", "2.txt");
    delete_regular_file(1, "first_dir", "1.txt");
    change_file_name(1, "first_dir", "2.txt", "3.txt");

    std::vector<std::string> test_files = {"3.txt"};
    EXPECT_EQ(get_list_files_in_dir(1, "first_dir"), test_files);

    test_files[0] = "2.txt";
    EXPECT_EQ(get_list_files_in_dir(1, "first_dir/second_dir"), test_files);
}


///---Work in tmp mode---///


TEST(DATA_BASE_TEST, WORK_WITH_TMP_FILES) {
    auto unique_id_1 = push_tmp_file(1, "06.06.06", "");
    auto unique_id_2 = push_tmp_file(1, "05.05.05", "type1234");
    auto unique_id_3 =  push_tmp_file(1, "04.04.04", "type1234");

    delete_tmp_files("23.09.09");

    EXPECT_EQ(check_access_on_tmp_file(unique_id_1, "qwertyui"), 0);
    EXPECT_EQ(check_access_on_tmp_file(unique_id_1, ""), 1);
    EXPECT_EQ(check_access_on_tmp_file(unique_id_2, "type1"), 0);
    EXPECT_EQ(check_access_on_tmp_file(unique_id_2, "type1234"), 1);
    EXPECT_EQ(check_access_on_tmp_file(unique_id_2, "type1234"), 1);
}


///---Access in group mode---///


TEST(DATA_BASE_TEST, GROUP_MODE) {
    try_register("Ivan", "ivan@mail.ru", "IVAN");
    try_register("Sergey", "sergey@mail.ru", "SERGEY");
    try_register("Peter", "peter@mail.ru", "PETER");

    create_group(1, "test_group_1");
    std::vector<std::pair<std::string, std::string>> list_users;

    add_group_member(1, "test_group_1", 2);
    list_users.emplace_back("Ivan", "ivan@mailru");
    add_group_member(1, "test_group_1", 3);
    list_users.emplace_back("Sergey", "sergey@mailru");
    add_group_member(1, "test_group_1", 4);
    list_users.emplace_back("Peter", "peter@mailru");

    ASSERT_EQ(list_users_in_group(1), list_users);

    delete_group_member(2, 1, 2);

    EXPECT_EQ(check_user_already_in_group(1, 2), 0);
    EXPECT_EQ(check_user_already_in_group(1, 3), 1);

    modifiers_t mode = {.additional_bit = 0, .user = 7, .group = 7, .other = 7};
    set_chmod(mode);
    auto set_modes = get_access_modifiers(1);
    EXPECT_EQ(set_modes.additional_bit, mode.additional_bit);
    EXPECT_EQ(set_modes.user, mode.user);
    EXPECT_EQ(set_modes.group, mode.group);
    EXPECT_EQ(set_modes.other, mode.other);

    check_access(1, 1, DELETE_GROUP);
    delete_group(1, 1);
}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
