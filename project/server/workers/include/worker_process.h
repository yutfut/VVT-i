#pragma once

#include <unistd.h>
#include <map>
#include <sys/epoll.h>
#include <csignal>

#include "client_connection.h"

#include <fs_worker.h>
#include <data_base.h>

extern bool is_hard_stop;
extern bool is_soft_stop;
extern bool is_soft_reload;

class WorkerProcess {
public:

    explicit WorkerProcess(int listen_sock, class ServerSettings *server_settings, std::vector<Log *> &vector_logs,
                            FsWorker &fs_worker,  DataBase &db_worker);

    ~WorkerProcess() = default;

    void run();

    void setup_signals();

    static void sighup_handler(int sig); // Handler for soft stop

    static void sigint_handler(int sig); // Handler for hard stop

    void write_to_logs(const std::string &message, bl::trivial::severity_level lvl);

private:
    typedef enum {
        INFO_HARD_STOP_DONE,
        INFO_SOFT_STOP_START,
        INFO_SOFT_STOP_DONE
    } log_messages_t;

    class ServerSettings *server_settings;

    int listen_sock;

    void message_to_log(log_messages_t log_type);

    std::vector<Log *> vector_logs;

    FsWorker &fs_worker; // TODO: нужна обработка файла конфигурации

    DataBase &db_worker; // TODO: нужна обработка файла конфигурации

};
