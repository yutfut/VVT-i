#pragma once

#include <fstream>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstdlib>
#include <unistd.h>
#include <csignal>
#include <fs_worker.h>
#include <data_base.h>
#include "main_server_settings.h"
#include "worker_process.h"

typedef boost::log::sources::severity_logger<boost::log::trivial::severity_level> logger_t;

extern int process_soft_stop;
extern int process_hard_stop;
extern int process_soft_reload;
extern int process_hard_reload;
extern int has_old_master_stopped;
extern pid_t new_master_pid;
extern std::string path_to_config;

typedef enum {
    SOFT_LEVEL,
    HARD_LEVEL,
    NULL_LEVEL
} action_level_t;

typedef enum {
    START_SERVER,
    RELOAD_SERVER,
} status_server_action;

typedef enum {
    EMPTY_MASTER,
    OLD_MASTER,
    NEW_MASTER
} master_status_t;

class Server {
public:
    Server(FsWorker& fs_worker, DataBase& db_worker, const std::string& config_filename = "settings/server.conf");

    bool start();

    bl::trivial::severity_level cast_types_logs_level(const std::string &lvl);

    void write_to_logs(std::string message, bl::trivial::severity_level lvl);

    bool daemonize(status_server_action server_action);

    bool bind_listen_sock();

    bool add_work_processes();

    bool fill_pid_file();

    bool delete_pid_file();

    static int setup_signals(); // Set handlers to signals

    static void sighup_handler(int sig, siginfo_t *info, void *param); // Handler for soft stop

    static void sigint_handler(int sig, siginfo_t *info, void *param); // Handler for hard stop

    static void sigpipe_handler(int sig); // Handler for soft reload

    static void sigalrm_handler(int sig); // Handler for hard reload

    static void sigchld_handler(int sig); // Handler for end of soft stop

    bool server_stop(action_level_t level);

    bool server_reload(action_level_t level);

    bool apply_config(action_level_t level);

private:
    std::string config_filename;

    int count_workflows = 0;

    pid_t old_master_process = 0, new_master_process = 0;

    std::vector<pid_t> workers_pid;

    std::string access_log_level = "debug", error_log_level = "error";

    Log access_log, error_log;

    std::vector<Log *> vector_logs;

    class ServerSettings server;

    int listen_sock = 0;

    MainServerSettings settings;

    FsWorker fs_worker;

    DataBase db_worker;
};
