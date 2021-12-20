//
// Created by Yut Fut on 20.12.2021.
//

#include <gtest/gtest.h>

#include "http_response.h"

TEST(HTTP_RESPONSE, HTTP_RESPONSE_TEST_OK) {
std::string result ="POST / HTTP/1.0\r\n"
                    "email: \r\n"
                    "password: \r\n"
                    "jwt: \r\n"
                    "current_directory: \r\n"
                    "work_directory: \r\n"
                    "key: \r\n"
                    "command: \r\n"
                    "filename: \r\n"
                    "message: OK\r\n"
                    "content-length: 0\r\n\r\n\r\n";
EXPECT_EQ(HTTPResponse::parser(result), 0);
}

TEST(HTTP_RESPONSE, HTTP_RESPONSE_TEST_FAIL) {
std::string result ="POST / HTTP/1.0\r\n"
                    "email: \r\n"
                    "password: \r\n"
                    "jwt: \r\n"
                    "current_directory: \r\n"
                    "work_directory: \r\n"
                    "key: \r\n"
                    "command: \r\n"
                    "filename: \r\n"
                    "message: FAIL\r\n"
                    "content-length: 0\r\n\r\n\r\n";
EXPECT_EQ(HTTPResponse::parser(result), -1);
}