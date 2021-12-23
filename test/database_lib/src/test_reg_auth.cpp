#include "../include/configurations.h"

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

    db.reg_auth.try_register("fart@gmail.com", "1234");
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