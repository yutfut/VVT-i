//
// Created by Yut Fut on 18.11.2021.
//

#include <gtest/gtest.h>

#include "client.h"

TEST(VALIDATION_TEST, VALIDATION_EMAIL_TEST_OK) {
    Client client;
    EXPECT_EQ(client.valid_email("command"), 0);
}

TEST(VALIDATION_TEST, VALIDATION_EMAIL_TEST_FAIL) {
    Client client;
    EXPECT_EQ(client.valid_email(""), -1);
}

TEST(VALIDATION_TEST, VALIDATION_PASSWORD_TEST_OK) {
    Client client;
    EXPECT_EQ(client.valid_password("command"), 0);
}

TEST(VALIDATION_TEST, VALIDATION_PASSWORD_TEST_FAIL) {
    Client client;
    EXPECT_EQ(client.valid_password(""), -1);
}

TEST(VALIDATION_TEST, VALIDATION_CHMOD_TEST_OK) {
    Client client;
    EXPECT_EQ(client.valid_chmod("command"), 0);
}

TEST(VALIDATION_TEST, VALIDATION_CHMOD_TEST_FAIL) {
    Client client;
    EXPECT_EQ(client.valid_chmod(""), -1);
}

TEST(VALIDATION_TEST, VALIDATION_DIRECTORY_TEST_OK) {
    Client client;
    EXPECT_EQ(client.valid_directory("command"), 0);
}

TEST(VALIDATION_TEST, VALIDATION_DIRECTORY_TEST_FAIL) {
    Client client;
    EXPECT_EQ(client.valid_directory(""), -1);
}

TEST(REGISTRATION_TEST, LOGIN_ON_SERVER_TEST_OK) {
    Client client;
    EXPECT_EQ(client.login_on_server(), 0);
}

TEST(REGISTRATION_TEST, LOGIN_ON_SERVER_TEST_FAIL) {
    Client client;
    EXPECT_EQ(client.login_on_server(), -1);
}

TEST(REGISTRATION_TEST, REGISTRATION_TEST_OK) {
    Client client;
    EXPECT_EQ(client.registration("command"), 0);
}

TEST(REGISTRATION_TEST, REGISTRATION_TEST_FAIL) {
    Client client;
    EXPECT_EQ(client.registration(""), -1);
}

TEST(REGISTRATION_TEST, LOGIN_TEST_OK) {
    Client client;
    EXPECT_EQ(client.login("command"), 0);
}

TEST(REGISTRATION_TEST, LOGIN_TEST_OK_WHERE_USER_LOGINED) {
    Client client;
    client.login("command");
    EXPECT_EQ(client.login("command"), 0);
}

TEST(REGISTRATION_TEST, LOGIN_TEST_FAIL) {
    Client client;
    EXPECT_EQ(client.login(""), -1);
}

TEST(REGISTRATION_TEST, LOGOUT_TEST_OK) {
    Client client;
    client.login("command");
    EXPECT_EQ(client.logout(), 0);
}

TEST(REGISTRATION_TEST, LOGOUT_TEST_OK_WHERE_USER_LOGOUTED) {
    Client client;
    EXPECT_EQ(client.logout(), 0);
}

TEST(REGISTRATION_TEST, LOGOUT_TEST_FAIL) {
    Client client;
    client.login("command");
    EXPECT_EQ(client.logout(), -1);
}

TEST(SERVER_TEST, GET_FROM_SERVER_TEST_OK) {
    Client client;
    EXPECT_EQ(client.get_from_server(), 0);
}

TEST(SERVER_TEST, GET_FROM_SERVER_TEST_FAIL) {
    Client client;
    EXPECT_EQ(client.get_from_server(), -1);
}

TEST(SERVER_TEST, SEND_ON_SERVER_TEST_OK) {
    Client client;
    EXPECT_EQ(client.send_on_server(), 0);
}

TEST(SERVER_TEST, SEND_ON_SERVER_TEST_FAIL) {
    Client client;
    EXPECT_EQ(client.send_on_server(), -1);
}

TEST(WORK_WITH_COMMAND_TEST, SEARCH_FILE_TEST_OK) {
    Client client;
    EXPECT_EQ(client.search_file("file_name"), 0);
}

TEST(WORK_WITH_COMMAND_TEST, SEARCH_FILE_TEST_FAIL) {
    Client client;
    EXPECT_EQ(client.search_file(""), -1);
}

TEST(WORK_WITH_COMMAND_TEST, WORK_WITH_FILE_TEST_OK) {
    Client client;
    EXPECT_EQ(client.work_with_file("command"), 0);
}

TEST(WORK_WITH_COMMAND_TEST, WORK_WITH_FILE_TEST_FAIL) {
    Client client;
    EXPECT_EQ(client.work_with_file(""), -1);
}

TEST(WORK_WITH_COMMAND_TEST, WORK_WITH_CHMOD_TEST_OK) {
    Client client;
    EXPECT_EQ(client.work_with_chmod("command"), 0);
}

TEST(WORK_WITH_COMMAND_TEST, WORK_WITH_CHMOD_TEST_FAIL) {
    Client client;
    EXPECT_EQ(client.work_with_chmod(""), -1);
}

TEST(WORK_WITH_COMMAND_TEST, WORK_WITH_DIRECTORY_TEST_OK) {
    Client client;
    EXPECT_EQ(client.work_with_directory("command"), 0);
}

TEST(WORK_WITH_COMMAND_TEST, WORK_WITH_DIRECTORY_TEST_FAIL) {
    Client client;
    EXPECT_EQ(client.work_with_directory(""), -1);
}

TEST(ROLE_COMMAND_TEST, ROLE_COMMAND_TEST_OK) {
    Client client;
    EXPECT_EQ(client.role_command("upload"), 0);
    EXPECT_EQ(client.role_command("chmod"), 0);
    EXPECT_EQ(client.role_command("mkdir"), 0);
    EXPECT_EQ(client.role_command("exit"), 1);
    EXPECT_EQ(client.role_command("login"), 0);
    EXPECT_EQ(client.role_command("logout"), 0);
    EXPECT_EQ(client.role_command("registration"), 0);
}

TEST(ROLE_COMMAND_TEST, ROLE_COMMAND_TEST_FAIL) {
    Client client;
    EXPECT_EQ(client.role_command(""), -1);
}

TEST(ROLE_COMMAND_TEST, VALIDATION_COMMAND_TEST_OK) {
    Client client;
    EXPECT_EQ(client.role_command("upload"), 0);
}

TEST(ROLE_COMMAND_TEST, VALIDATION_COMMAND_TEST_FAIL) {
    Client client;
    EXPECT_EQ(client.role_command(""), -1);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}