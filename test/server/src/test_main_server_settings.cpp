#include <gtest/gtest.h>

#include <main_server_settings.h>

TEST(TEST_CONFIG, HANDLE_EMPTY_CONFIG) {
    auto settings = MainServerSettings("test_settings/default.conf");

    EXPECT_EQ(settings.get_count_workflows(), 4);
    EXPECT_EQ(settings.get_access_log_filename(), "access.log");
    EXPECT_EQ(settings.get_error_log_filename(), "error.log");

    EXPECT_EQ(settings.get_server().get_port(), 80);
    EXPECT_EQ(settings.get_server().get_servername(), "127.0.0.1");
    EXPECT_EQ(settings.get_server().get_fs_root(), "/");

    EXPECT_EQ(settings.get_server().get_database().user, "postgres");
    EXPECT_EQ(settings.get_server().get_database().password, "postgres");
    EXPECT_EQ(settings.get_server().get_database().host, "localhost");
    EXPECT_EQ(settings.get_server().get_database().port, 5432);
    EXPECT_EQ(settings.get_server().get_database().dbname, "VVT-i");
}

TEST(TEST_CONFIG, HANDLE_INVALID_CONFIG) {
    EXPECT_ANY_THROW({ auto settings = MainServerSettings("test_settings/invalid.conf"); });
}

TEST(TEST_CONFIG, HandleConfig) {
    auto settings = MainServerSettings("test_settings/fs_worker_lib.conf");

    EXPECT_EQ(settings.get_count_workflows(), 2);
    EXPECT_EQ(settings.get_access_log_filename(), "test_access.log");
    EXPECT_EQ(settings.get_error_log_filename(), "test_error.log");

    EXPECT_EQ(settings.get_server().get_port(), 3000);
    EXPECT_EQ(settings.get_server().get_servername(), "192.168.0.1");
    EXPECT_EQ(settings.get_server().get_fs_root(), "/path");

    EXPECT_EQ(settings.get_server().get_database().user, "test_user");
    EXPECT_EQ(settings.get_server().get_database().password, "test_password");
    EXPECT_EQ(settings.get_server().get_database().host, "192.168.0.2");
    EXPECT_EQ(settings.get_server().get_database().port, 4000);
    EXPECT_EQ(settings.get_server().get_database().dbname, "name");
}
