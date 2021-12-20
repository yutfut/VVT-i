//
// Created by Yut Fut on 18.11.2021.
//

#include <gtest/gtest.h>

#include "validation.h"

TEST(VALIDATION_TEST, VALIDATION_EMAIL_TEST_OK) {
    EXPECT_EQ(Validator::validate_email("m@m.m"), 0);
}

TEST(VALIDATION_TEST, VALIDATION_EMAIL_TEST_FAIL) {
    EXPECT_EQ(Validator::validate_email(std::string {}), -1);
}

TEST(VALIDATION_TEST, VALIDATION_PASSWORD_TEST_OK) {
    EXPECT_EQ(Validator::validate_password("Qw12##12"), 0);
}

TEST(VALIDATION_TEST, VALIDATION_PASSWORD_TEST_FAIL) {
EXPECT_EQ(Validator::validate_password(std::string {}), -1);
}

TEST(VALIDATION_TEST, VALIDATION_KEY_TEST_OK) {
EXPECT_EQ(Validator::validate_key("cb014b74-d8a2-4c2a-9ec3-dde878151edc"), 0);
}

TEST(VALIDATION_TEST, VALIDATION_KEY_TEST_FAIL) {
EXPECT_EQ(Validator::validate_key(std::string {}), -1);
}