#include <iostream>
#include "data_base.h"
#include <string>
#include <ctime>

int main() {

    database_configuration_t config = {.user = USER, .password = PASSWORD, .host = HOST, .port = PORT, .dbname = "vvti"};

    DataBase db(config);
//    DataBase db_1(config);

    db.init();
//     db.not_auth_mode.add_file("first_file", "pass1");
//     db.not_auth_mode.add_file("first_file", "pass1");
    // db.not_auth_mode.add_file("first_file", "pass1");
    // std::cout << db.not_auth_mode.get_upload_file_date("d659ca4b-904c-4e68-b05a-d4624cbddeee", "pass1").filename << "\n";
    // std::cout << db.not_auth_mode.get_upload_file_date("5ff6f1d0-58a7-493a-9a6b-f3eee249f4c9", "pass1").upload_date << "\n";
    // std::cout << db.not_auth_mode.get_upload_file_date("e07dbace-20ef-413d-965d-6d458a2a842c", "pass1").upload_date << "\n";
    // db.not_auth_mode.delete_files_by_date("2021-12-01");

    // std::cout << db.reg_auth.is_email_free("email") << "\n";
    db.reg_auth.try_register("email", "pass");
    db.reg_auth.try_register("email1", "pass");
    db.reg_auth.try_register("email2", "pass");
    db.reg_auth.try_register("email3", "pass");
    // std::cout << db.reg_auth.is_email_free("email") << "\n";

    // std::cout << db.reg_auth.try_auth("email", "past") << "\n";
    // std::cout << db.reg_auth.try_auth("emai", "pass") << "\n";
    // std::cout << db.reg_auth.try_auth("email", "pass") << "\n";

    // std::cout << db.reg_auth.get_id_auth_user("emai") << "\n";
    // int id =  db.reg_auth.get_id_auth_user("email");
    // std::cout << id << "\n";

    // std::cout << db.reg_auth.get_email(id);

    // std::cout << db.single_auth_mode.add_file(1, "1", "1.txt");
    // std::cout << db.single_auth_mode.is_dir_name_free(1, "1");
    // std::cout << db.single_auth_mode.is_filename_free(1, "1", "2.txt");
    // std::cout << db.single_auth_mode.is_filename_free(1, "1", "1.txt");
    // std::cout << db.single_auth_mode.delete_file(1, "1", "2.txt");
    // std::cout << db.single_auth_mode.delete_file(1, "1", "1.txt");
    // std::cout << db.single_auth_mode.add_file(1, "1", "1.txt");
    // std::cout << db.single_auth_mode.add_file(1, "1", "11.txt");

    // std::cout << db.single_auth_mode.change_filename(1, "1", "1.txt", "2.txt");
    // std::cout << db.single_auth_mode.change_filename(1, "1", "2.txt", "1.txt");
    // std::cout << db.single_auth_mode.change_filename(1, "1", "1.txt", "3.txt");

    // std::cout << db.single_auth_mode.delete_file (1, "1", "4.txt");
    // std::cout << db.single_auth_mode.delete_file(1, "1", "3.txt");
    // std::cout << db.single_auth_mode.delete_file(1, "1", "2.txt");
    // db.single_auth_mode.create_directory(1, "1", "5");
    // db.single_auth_mode.create_directory(1, "1", "8");
    // db.single_auth_mode.create_directory(1, "1", "6");
    // db.single_auth_mode.create_directory(1, "1", "2");
    // db.single_auth_mode.create_directory(1, "1", "3");
    // std::cout << db.single_auth_mode.add_file(1, "1", "11.txt");
    // std::cout << db.single_auth_mode.add_file(1, "1", "6.txt");
    // std::cout << db.single_auth_mode.add_file(1, "1", "3.txt");
    // std::cout << db.single_auth_mode.add_file(1, "1", "7.txt");
    // std::cout << db.single_auth_mode.add_file(1, "1", "2.txt");
    // std::cout << db.single_auth_mode.add_file(1, "1/2", "not need to cout.txt");

    // std::cout << db.single_auth_mode.get_list_files_in_dir(1, "1");


    std::time_t t = std::time(0);   // get time now
    std::tm *now = std::localtime(&t);
    std::string curr_date;
    curr_date += std::to_string(now->tm_year + 1900) + "-" + std::to_string(now->tm_mon + 1)
                 + "-" + std::to_string(now->tm_mday);

    auto id = db.reg_auth.try_register("user_1", "new_pass");

    // std::cout << "/first_dir\n\n";

    db.single_auth_mode.create_directory(id, "/", "first_dir");
    db.single_auth_mode.create_directory(id, "/first_dir", "second_dir");

    db.single_auth_mode.add_file(id, "/first_dir", "1.txt");
    db.single_auth_mode.add_file(id, "/first_dir", "2.txt");
    db.single_auth_mode.add_file(id, "/first_dir/second_dir", "2.txt");
    db.single_auth_mode.add_file(id, "/first_dir", "3.txt");

    db.single_auth_mode.change_filename(id, "/first_dir", "2.txt", "4.txt");

    std::string files_in_dir = fmt::format("-{0}  {2}  {1}\n-{0}  {3}  {1}\n-{0}  {4}  {1}\n", BASE_ACCESS_LVL,
                                           curr_date, "1.txt", "3.txt", "4.txt");

    std::string dirs_in_dir = fmt::format("d{0}  {2}  {1}\n", BASE_ACCESS_LVL, curr_date, "second_dir");

    std::cout << "need result:\n\n" << files_in_dir + dirs_in_dir << "\n\nmy result:\n\n";

    std::cout << db.single_auth_mode.get_list_files_in_dir(id, "/first_dir");

    std::cout << "rmdir!!1\n\n";

    db.single_auth_mode.rmdir(id, "/", "first_dir");

    std::cout << db.single_auth_mode.get_list_files_in_dir(id, "/first_dir");

    return 0;
}
