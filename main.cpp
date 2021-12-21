#include "server.h"
#include <iostream>

int main() {
    FsWorker fs_worker;

    database_configuration_t db_conf = {"postgres", "postgres", "localhost", "5432", "vvti"};

    DataBase db_worker(db_conf);
    db_worker.init();

    Server server(fs_worker, db_worker, "./settings/server.conf");
    if (!server.start()) {
        return -1;
    }

    return 0;
}
