#include <iostream>
#include "data_base.h"
#include <string>

int main() {
    DataBase db("postgres", "postgres", "5432", "localhost", "");
    db.init();
    // db.not_auth_mode.add_unauth_user_file("first_file", "pass1");
    // db.not_auth_mode.add_unauth_user_file("first_file", "pass1");
    db.not_auth_mode.add_unauth_user_file("first_file", "pass1");
    // std::cout << db.not_auth_mode.get_upload_file_date("d659ca4b-904c-4e68-b05a-d4624cbddeee", "pass1").filename << "\n";
    std::cout << db.not_auth_mode.get_upload_file_date("5ff6f1d0-58a7-493a-9a6b-f3eee249f4c9", "pass1").upload_date << "\n";
    // std::cout << db.not_auth_mode.get_upload_file_date("e07dbace-20ef-413d-965d-6d458a2a842c", "pass1").upload_date << "\n";
    // db.not_auth_mode.delete_files_by_date("2021-12-01");

    // std::cout << db.reg_auth.is_email_free("email") << "\n";
    db.reg_auth.try_register("name", "email", "pass");
    // std::cout << db.reg_auth.is_email_free("email") << "\n";

    std::cout << db.reg_auth.try_auth("email", "past") << "\n";
    std::cout << db.reg_auth.try_auth("emai", "pass") << "\n";
    std::cout << db.reg_auth.try_auth("email", "pass") << "\n";

    // std::cout << db.reg_auth.get_id_auth_user("emai") << "\n";
    // int id =  db.reg_auth.get_id_auth_user("email");
    // std::cout << id << "\n";
    
    // std::cout << db.reg_auth.get_email(id);

    // std::cout << db.single_auth_mode.add_auth_user_file(1, "1", "1.txt");
    // std::cout << db.single_auth_mode.is_dir_name_free(1, "1");
    // std::cout << db.single_auth_mode.is_filename_free(1, "1", "2.txt");
    // std::cout << db.single_auth_mode.is_filename_free(1, "1", "1.txt");
    // std::cout << db.single_auth_mode.delete_auth_user_file(1, "1", "2.txt");
    // std::cout << db.single_auth_mode.delete_auth_user_file(1, "1", "1.txt");
    // std::cout << db.single_auth_mode.add_auth_user_file(1, "1", "1.txt");
    // std::cout << db.single_auth_mode.add_auth_user_file(1, "1", "2.txt");

    // std::cout << db.single_auth_mode.change_filename(1, "1", "1.txt", "2.txt");
    // std::cout << db.single_auth_mode.change_filename(1, "1", "2.txt", "1.txt");
    // std::cout << db.single_auth_mode.change_filename(1, "1", "1.txt", "3.txt");

    // std::cout << db.single_auth_mode.delete_auth_user_file(1, "1", "4.txt");
    // std::cout << db.single_auth_mode.delete_auth_user_file(1, "1", "3.txt");
    // std::cout << db.single_auth_mode.delete_auth_user_file(1, "1", "2.txt");

    std::cout << "Hello, World!" << std::endl;


    return 0;
}
