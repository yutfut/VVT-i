#include "configurations.h"

TEST(REG_AUTH_MODE, CHECK_EMAIL_FREE) {
    check_and_create_db();

    DataBase db(VALID_CONF);
    db.init();

    db.reg_auth.try_register("fart@gmail.com", "1234");

    EXPECT_EQ(db.reg_auth.is_email_free("fart@gmail.co"), 1);
    EXPECT_EQ(db.reg_auth.is_email_free("fart@gmail.com"), 0);
}


TEST(REG_AUTH_MODE, SAME_EMAILS) {
    DataBase db(VALID_CONF);

    db.reg_auth.try_register("fart@gmail.com", "1234");
    EXPECT_EQ(db.reg_auth.try_register("fart@gmail.com", "pass"), -1);
}


TEST(REG_AUTH_TEST, NOT_VALID_DATA_TO_AUTH) {
    DataBase db(VALID_CONF);

    db.reg_auth.try_register("fart@gmail.com", "1234");
    EXPECT_EQ(db.reg_auth.try_auth("fart@gmail.com", "123"), -1);
    EXPECT_EQ(db.reg_auth.try_auth("fart@gmail.co", "1234"), -1);
}


TEST(REG_AUTH_MODE, VALID_DATA_TO_AUTH) {
    DataBase db(VALID_CONF);

    EXPECT_EQ(db.reg_auth.try_auth("fart@gmail.com", "1234"), 1);
}


TEST(REG_AUTH_MODE, GET_EMAIL) {
    DataBase db(VALID_CONF);

    auto id = db.reg_auth.get_id_auth_user("fart@gmail.com");
    EXPECT_EQ(db.reg_auth.get_email(id), "fart@gmail.com");
}

TEST(REG_AUTH_MODE, DELETE_USER) {
    DataBase db(VALID_CONF);

    db.reg_auth.try_register("delete_user@gmail.com", "");
    auto id = db.reg_auth.get_id_auth_user("delete_user@gmail.com");
    EXPECT_EQ(db.reg_auth.delete_user(id), true);
    EXPECT_EQ(db.reg_auth.is_email_free("delete_user@gmail.com"), true);
}
