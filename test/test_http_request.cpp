//
// Created by Yut Fut on 20.12.2021.
//

#include <gtest/gtest.h>

#include "http_request.h"

TEST(HTTP_REQUEST, HTTP_REQUEST_TEST_OK) {
    std::string result ="POST / HTTP/1.0\r\n"
                        "email: \r\n"
                        "password: \r\n"
                        "jwt: \r\n"
                        "current_directory: \r\n"
                        "work_directory: \r\n"
                        "key: \r\n"
                        "command: \r\n"
                        "filename: \r\n"
                        "content-length: 0\r\n\r\n\r\n";
    EXPECT_EQ(HTTPRequest::create_message(std::string {},
                                          std::string {},
                                          std::string {},
                                          std::string {},
                                          std::string {},
                                          std::string {},
                                          std::string {},
                                          std::string {}), result);
}

TEST(HTTP_REQUEST, HTTP_REQUEST_WITH_DATA_TEST_OK) {
std::string result ="POST / HTTP/1.0\r\n"
                    "email: email@mail.ru\r\n"
                    "password: Qw12##12\r\n"
                    "jwt: \r\n"
                    "current_directory: /\r\n"
                    "work_directory: \r\n"
                    "key: \r\n"
                    "command: command\r\n"
                    "filename: \r\n"
                    "content-length: 0\r\n\r\n\r\n";
EXPECT_EQ(HTTPRequest::create_message("email@mail.ru",
                                      "Qw12##12",
                                      std::string {},
                                      "/",
                                      std::string {},
                                      std::string {},
                                      std::string {},
                                      "command"), result);
}

TEST(HTTP_REQUEST, HTTP_REQUEST_WITH_DATA_FILE_OK) {
std::string result ="POST / HTTP/1.0\r\n"
                    "email: email@mail.ru\r\n"
                    "password: Qw12##12\r\n"
                    "jwt: \r\n"
                    "current_directory: /\r\n"
                    "work_directory: \r\n"
                    "key: \r\n"
                    "command: upload\r\n"
                    "filename: main.py\r\n"
                    "content-length: 52\r\n\r\n"
                    "print('vvt-i')\nfor i in range(1, 100):\n    print(i)\n\r\n";
EXPECT_EQ(HTTPRequest::create_message("email@mail.ru",
                                      "Qw12##12",
                                      std::string {},
                                      "/",
                                      std::string {},
                                      std::string {},
                                      "main.py",
                                      "upload"), result);
}