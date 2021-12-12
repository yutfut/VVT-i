#include <iostream>
#include "data_base.h"

int main() {
    DataBase db("postgres", "postgres", "5432", "localhost", "");
    db.init();
    std::cout << db.not_auth_mode.get_upload_file_date("b6882d0e-eedd-45c1-a146-44cd70f02a4d", "").filename << "\n";
    std::cout << db.not_auth_mode.get_upload_file_date("b6882d0e-eedd-45c1-a146-44cd70f02a4d", "").filename << "\n";
    std::cout << db.not_auth_mode.get_upload_file_date("b6882d0e-eedd-45c1-a146-44cd70f02a4d", "").filename << "\n";
    std::cout << db.not_auth_mode.get_upload_file_date("b6882d0e-eedd-45c1-a146-44cd70f02a4d", "").filename << "\n";

    return 0;
}