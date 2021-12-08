#include "server.h"
#include <iostream>

int main() {
    FsWorker fs_worker;
    DataBase db_worker("postgres", "postgres", "5432", "localhost", "");
    db_worker.init();
    std::cout<<"11234445\n";
    Server server( fs_worker, db_worker,"settings/server.conf");
    std::cout<<"1234445\n";
    if (!server.start()) {
        return -1;
    }

    return 0;
}
