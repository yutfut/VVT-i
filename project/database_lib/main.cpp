#include <iostream>
#include "data_base.h"

int main() {
    DataBase db("postgres", "postgres", "5432", "localhost", "");
    db.init();
    db.not_auth_mode.add_unauth_user_file("first_file", "pass1");
    db.not_auth_mode.delete_unauth_user_files("2021-12-01");

    std::cout << "Hello, World!" << std::endl;
    return 0;
}
