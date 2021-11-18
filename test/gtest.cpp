//
// Created by Yut Fut on 18.11.2021.
//

#include <gtest/gtest.h>

extern "C" {
    #include "../include/client.h"
}

TEST(TEST, TEST) {
    int result = send_on_server();
    EXPECT_EQ(result, 0);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}