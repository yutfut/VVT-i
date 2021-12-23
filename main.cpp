#include "server.h"
#include <iostream>

int main() {
    FsWorker fs_worker;
    DataBase db_worker({"postgres", "postgres", "5432", "localhost", "vvti"});
    db_worker.init();

    Server server( fs_worker, db_worker,"./settings/server.conf");
    if (!server.start()) {
        return -1;
    }

    return 0;
}
