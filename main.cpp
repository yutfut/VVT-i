#include "server.h"
#include <iostream>

int main() {
    Server server("./settings/server.conf");
    if (!server.start()) {
        return -1;
    }

    return 0;
}
