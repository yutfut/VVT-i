#include "configurations.h"

TEST(DATABASE_CONNECTION, NOT_VALID_CONFIG) {
    check_and_create_db();

    EXPECT_ANY_THROW({ DataBase db_1(EMPTY_HOST_CONF); });

    EXPECT_ANY_THROW({ DataBase db_2(NOT_EXIST_DBNAME_CONF); });
}


TEST(DATABASE_CONNECTION, VALID_CONFIG) {
    check_and_create_db();

    DataBase db_2(VALID_CONF);
    EXPECT_EQ(db_2.init(), 0);
}


TEST(DATABASE_CONNECTION, SET_CONNECTION) {
    check_and_create_db();

    DataBase db_2(VALID_CONF);

    EXPECT_EQ(db_2.get_connection().get_user_name(), VALID_CONF.user);
    EXPECT_EQ(db_2.get_connection().get_password(), VALID_CONF.password);
    EXPECT_EQ(db_2.get_connection().get_host(), VALID_CONF.host);
    EXPECT_EQ(db_2.get_connection().get_port(), VALID_CONF.port);
    EXPECT_EQ(db_2.get_connection().get_dbname(), VALID_CONF.dbname);
}


TEST(INIT_DATA_BASE, VALID_CONFIG) {
    check_and_create_db();

    DataBase db_2(VALID_CONF);
    EXPECT_EQ(db_2.init(), 0);
}
