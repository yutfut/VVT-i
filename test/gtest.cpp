//
// Created by Yut Fut on 18.11.2021.
//

#include <gtest/gtest.h>

#include "client.h"
#include "command.h"
#include "validation.h"
#include "http_request.h"
#include "http_response.h"

TEST(VALIDATION_TEST, VALIDATION_EMAIL_TEST_OK) {
    EXPECT_EQ(Validator::validate_email("command"), 0);
}

TEST(VALIDATION_TEST, VALIDATION_EMAIL_TEST_FAIL) {
    EXPECT_EQ(Validator::validate_email(""), -1);
}

TEST(VALIDATION_TEST, VALIDATION_PASSWORD_TEST_OK) {
    EXPECT_EQ(Validator::validate_password("command"), 0);
}

TEST(VALIDATION_TEST, VALIDATION_PASSWORD_TEST_FAIL) {
    EXPECT_EQ(Validator::validate_password(""), -1);
}

TEST(VALIDATION_TEST, VALIDATION_CHMOD_TEST_OK) {
    EXPECT_EQ(Validator::validate_chmod("command"), 0);
}

TEST(VALIDATION_TEST, VALIDATION_CHMOD_TEST_FAIL) {
    EXPECT_EQ(Validator::validate_chmod(""), -1);
}

TEST(VALIDATION_TEST, VALIDATION_DIRECTORY_TEST_OK) {
    EXPECT_EQ(Validator::validate_directory("command"), 0);
}

TEST(VALIDATION_TEST, VALIDATION_DIRECTORY_TEST_FAIL) {
    EXPECT_EQ(Validator::validate_directory(""), -1);
}

TEST(REGISTRATION_TEST, REGISTRATION_TEST_OK) {
    User user;
    EXPECT_EQ(user.register_user("command"), 0);
}

TEST(REGISTRATION_TEST, REGISTRATION_TEST_FAIL) {
    User user;
    EXPECT_EQ(user.register_user(""), -1);
}

TEST(REGISTRATION_TEST, LOGIN_TEST_OK) {
    User user;
    EXPECT_EQ(user.login("command"), 0);
}

TEST(REGISTRATION_TEST, LOGIN_TEST_OK_WHERE_USER_LOGINED) {
    User user;
    user.login("command");
    EXPECT_EQ(user.login("command"), 0);
}

TEST(REGISTRATION_TEST, LOGIN_TEST_FAIL) {
    User user;
    EXPECT_EQ(user.login(""), -1);
}

TEST(REGISTRATION_TEST, LOGOUT_TEST_OK) {
    User user;
    user.login("command");
    EXPECT_EQ(user.logout(), 0);
}

TEST(REGISTRATION_TEST, LOGOUT_TEST_OK_WHERE_USER_LOGOUTED) {
    User user;
    EXPECT_EQ(user.logout(), 0);
}

TEST(REGISTRATION_TEST, LOGOUT_TEST_FAIL) {
    User user;
    user.login("command");
    EXPECT_EQ(user.logout(), -1);
}

TEST(SERVER_TEST, GET_FROM_SERVER_TEST_OK) {
    HTTPResponse client;
    EXPECT_EQ(client.get(), 0);
}

TEST(SERVER_TEST, GET_FROM_SERVER_TEST_FAIL) {
    HTTPResponse client;
    EXPECT_EQ(client.get(), -1);
}

TEST(SERVER_TEST, SEND_ON_SERVER_TEST_OK) {
    HTTPRequest client;
    EXPECT_EQ(client.send(), 0);
}

TEST(SERVER_TEST, SEND_ON_SERVER_TEST_FAIL) {
    HTTPRequest client;
    EXPECT_EQ(client.send(), -1);
}

TEST(WORK_WITH_COMMAND_TEST, SEARCH_FILE_TEST_OK) {
    Command client;
    EXPECT_EQ(client.search_file("file_name"), 0);
}

TEST(WORK_WITH_COMMAND_TEST, SEARCH_FILE_TEST_FAIL) {
    Command client;
    EXPECT_EQ(client.search_file(""), -1);
}

TEST(WORK_WITH_COMMAND_TEST, WORK_WITH_FILE_TEST_OK) {
    Command client;
    EXPECT_EQ(client.work_with_file("command"), 0);
}

TEST(WORK_WITH_COMMAND_TEST, WORK_WITH_FILE_TEST_FAIL) {
    Command client;
    EXPECT_EQ(client.work_with_file(""), -1);
}

TEST(WORK_WITH_COMMAND_TEST, WORK_WITH_CHMOD_TEST_OK) {
    Command client;
    EXPECT_EQ(client.work_with_chmod("command"), 0);
}

TEST(WORK_WITH_COMMAND_TEST, WORK_WITH_CHMOD_TEST_FAIL) {
    Command client;
    EXPECT_EQ(client.work_with_chmod(""), -1);
}

TEST(WORK_WITH_COMMAND_TEST, WORK_WITH_DIRECTORY_TEST_OK) {
    Command client;
    EXPECT_EQ(client.work_with_directory("command"), 0);
}

TEST(WORK_WITH_COMMAND_TEST, WORK_WITH_DIRECTORY_TEST_FAIL) {
    Command client;
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