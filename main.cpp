#include "server.h"

int main() {
    Server server("settings/server.conf");
    if (!server.start()) {
        return -1;
    }

    return 0;
}
