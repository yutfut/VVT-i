#include "../include/configurations.h"

TEST(DATA_BASE_TEST, DATA_BASE) {

    check_and_create_db();

    EXPECT_ANY_THROW({
                         DataBase db_1(EMPTY_HOST_CONF);
                     });

    DataBase db_2(VVTI_CONF);
    EXPECT_EQ(db_2.init(), 0);

    EXPECT_ANY_THROW({
                         DataBase db_3(NOT_EXIST_DBNAME_CONF);
                     });
}
