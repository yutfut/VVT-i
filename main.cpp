#include <iostream>
#include "data_base.h"
#include <string>

int main() {
    DataBase db("postgres", "postgres", "5432", "localhost", "");
    db.init();
    db.not_auth_mode.add_unauth_user_file("first_file", "pass1");
    db.not_auth_mode.add_unauth_user_file("first_file", "pass1");
    db.not_auth_mode.add_unauth_user_file("first_file", "pass1");
    std::cout << db.not_auth_mode.get_upload_file_date("d659ca4b-904c-4e68-b05a-d4624cbddeee", "pass1").filename << "\n";
    std::cout << db.not_auth_mode.get_upload_file_date("d659ca4b-904c-4e68-b05a-d4624cbddeee", "pass1").upload_date << "\n";
    std::cout << db.not_auth_mode.get_upload_file_date("e07dbace-20ef-413d-965d-6d458a2a842c", "pass1").upload_date << "\n";
    // db.not_auth_mode.delete_files_by_date("2021-12-01");

    std::cout << "Hello, World!" << std::endl;


    return 0;
}
