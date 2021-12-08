#include "server.h"

#define BACKLOG 256

int process_soft_stop = 0;
int process_hard_stop = 0;
int process_soft_reload = EMPTY_MASTER;
int process_hard_reload = EMPTY_MASTER;
int has_old_master_stopped = 0;

pid_t new_master_pid = 0;
// TODO: delete cout
Server::Server(FsWorker& fs_worker, DataBase& db_worker,const std::string& config_filename): fs_worker(fs_worker), db_worker(db_worker) {
    std::cout<<__FILE__<< __LINE__;
    this->config_filename = config_filename;
    std::cout<<__FILE__<< __LINE__;
    this->settings = MainServerSettings(this->config_filename);
    std::cout<<__FILE__<< __LINE__;
    this->count_workflows = this->settings.get_count_workflows();
    std::cout<<__FILE__<< __LINE__;
    this->server = this->settings.get_server();
    std::cout<<__FILE__<< __LINE__;

    this->error_log = Log(this->settings.get_error_log_filename(), true,
        this->cast_types_logs_level(error_log_level));
    std::cout<<__FILE__<< __LINE__;
    this->access_log = Log(this->settings.get_access_log_filename(), true,
        this->cast_types_logs_level(access_log_level));
    std::cout<<__FILE__<< __LINE__;

    this->vector_logs.push_back(&error_log);
    std::cout<<__FILE__<< __LINE__;
    this->vector_logs.push_back(&access_log);
    std::cout<<__FILE__<< __LINE__;

    db_worker.init();
    std::cout<<__FILE__<< __LINE__;

    this->write_to_logs("SERVER STARTING...", INFO);
}

bl::trivial::severity_level Server::cast_types_logs_level(const std::string& lvl) {
    if (lvl == "info") {
        return INFO;
    }
    if (lvl == "debug") {
        return DEBUG;
    }
    if (lvl == "trace") {
        return TRACE;
    }
    return ERROR;
}

void Server::write_to_logs(std::string message, bl::trivial::severity_level lvl) {
    for (auto& vector_log : this->vector_logs) {
        vector_log->log(message, lvl);
    }
}

bool Server::daemonize(status_server_action server_action) {
    if (server_action == START_SERVER) {
        close(STDIN_FILENO);
        close(STDOUT_FILENO);
        close(STDERR_FILENO);

        pid_t pid = fork();

        if (pid == -1) {
            return false;
        }

        // Return 0 for child and pid for master
        if (pid != 0) {
            exit(0);
        }

        this->old_master_process = getpid();
        setsid();
    }

    if (server_action == RELOAD_SERVER) {
        pid_t pid_child_of_old_master = fork();

        if (pid_child_of_old_master == -1) {
            return false;
        }

        // Check it isn't old master
        if (getpid() == this->old_master_process) {
            return true;
        }

        pid_t pid_to_finish = getpid();

        pid_t new_master_pid = fork();

        if (new_master_pid == -1) {
            return false;
        }
        if (getpid() == pid_to_finish) {
            exit(0);
        }

        this->new_master_process = getpid();
        setsid();
    }

    return true;
}

bool Server::fill_pid_file() {
    std::ofstream stream_to_pid_file;
    stream_to_pid_file.open("pid_file.txt", std::ios::out);

    if (!stream_to_pid_file.is_open()) {
        return false;
    }

    stream_to_pid_file << getpid() << std::endl;

    for (auto& i : this->workers_pid) {
        stream_to_pid_file << i << std::endl;
    }

    stream_to_pid_file.close();

    return true;
}

bool Server::delete_pid_file() {
    return remove("pid_file.txt");
}

bool Server::add_work_processes() {
    int count_work_processes = this->settings.get_count_workflows();

    if (count_work_processes <= 0) {
        write_to_logs("COUNT WORKFLOWS NEED BE MORE 0", ERROR);
        return false;
    }

    this->workers_pid.clear();

    for (int i = 0; i < count_work_processes; ++i) {
        pid_t pid = fork();
        if (pid == -1) {
            this->write_to_logs("ERROR FORK", ERROR);
            return false;
        }
        if (pid != 0) {
            this->workers_pid.push_back(pid);
        }
        else {
            WorkerProcess worker(this->listen_sock, &server, vector_logs, fs_worker, db_worker);
            worker.run();
            break;
        }
    }

    return true;
}

bool Server::start() {
    if (!this->daemonize(START_SERVER)) {
        this->write_to_logs("ERROR IN SERVER DAEMONIZE", ERROR);
        return false;
    }

    if (!this->bind_listen_sock()) {
        this->write_to_logs("ERROR IN BIND SOCKET", ERROR);
        return false;
    }

    if (!this->add_work_processes()) {
        this->write_to_logs("ERROR IN ADDING WORK PROCESSES", ERROR);
        return false;
    }

    if (!this->fill_pid_file()) {
        this->write_to_logs("ERROR IN FILL PID FILE", ERROR);
        return false;
    }

    this->write_to_logs("Worker processes (" +
        std::to_string(this->workers_pid.size()) + ") successfully started",
        INFO);

    this->setup_signals();

    while (true) {
        if (process_soft_stop == 1) {
            this->server_stop(SOFT_LEVEL);
            return true;
        }

        if (process_hard_stop == 1) {
            this->server_stop(HARD_LEVEL);
            return true;
        }

        if (process_soft_reload == OLD_MASTER) {
            if (!this->server_reload(SOFT_LEVEL)) {
                this->write_to_logs("ERROR SOFT RELOAD", ERROR);
                this->server_stop(HARD_LEVEL);
                return false;
            }
        }

        if (process_hard_reload == OLD_MASTER) {
            if (!this->server_reload(HARD_LEVEL)) {
                this->write_to_logs("ERROR HARD RELOAD", ERROR);
                this->server_stop(HARD_LEVEL);
                return false;
            }
        }

        sleep(1);
    }

    return true;
}

void Server::sighup_handler(int sig, siginfo_t* info, void* param) {
    process_soft_stop = 1;
    new_master_pid = info->si_pid;
}

void Server::sigint_handler(int sig, siginfo_t* info, void* param) {
    process_hard_stop = 1;
    new_master_pid = info->si_pid;
}

void Server::sigpipe_handler(int sig) {
    process_soft_reload = OLD_MASTER;
}

void Server::sigalrm_handler(int sig) {
    process_hard_reload = OLD_MASTER;
}

void Server::sigchld_handler(int sig) {
    has_old_master_stopped = 1;
}

int Server::setup_signals() {
    struct sigaction act;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;

    act.sa_handler = sigpipe_handler;
    sigaction(SIGPIPE, &act, nullptr);

    act.sa_flags = SA_SIGINFO;
    act.sa_handler = NULL;
    act.sa_sigaction = sighup_handler;
    sigaction(SIGHUP, &act, nullptr);

    act.sa_flags = SA_SIGINFO;
    act.sa_handler = NULL;
    act.sa_sigaction = sigint_handler;
    sigaction(SIGINT, &act, nullptr);

    act.sa_flags = 0;
    act.sa_sigaction = NULL;
    act.sa_handler = sigalrm_handler;
    sigaction(SIGALRM, &act, nullptr);

    act.sa_handler = sigchld_handler;
    sigaction(SIGCHLD, &act, nullptr);

    return 0;
}

bool Server::server_stop(action_level_t level) {
    if (level == HARD_LEVEL) {
        this->write_to_logs("HARD SERVER STOP...", WARNING);
        if (process_hard_reload == OLD_MASTER) {
            close(this->listen_sock);
        }

        int status;
        for (auto& i : this->workers_pid) {
            kill(i, SIGINT);
        }
        for (auto& i : this->workers_pid) {
            waitpid(i, &status, 0);
        }

        if (process_hard_reload == NEW_MASTER) {
            this->write_to_logs("OLD MASTER STOPPED! PID " + std::to_string(getpid()), INFO);
            kill(new_master_pid, SIGCHLD);
        }
        else {
            this->write_to_logs("SERVER STOPPED!", INFO);
            this->delete_pid_file();
        }

        exit(0);
    }

    if (level == SOFT_LEVEL) {
        this->write_to_logs("SOFT SERVER STOP...", WARNING);

        int status;
        for (auto& i : this->workers_pid) {
            kill(i, SIGHUP);
        }
        for (auto& i : this->workers_pid) {
            waitpid(i, &status, 0);
        }

        if (process_soft_reload == NEW_MASTER) {
            this->write_to_logs("OLD MASTER STOPPED! PID " + std::to_string(getpid()), INFO);
            kill(new_master_pid, SIGCHLD);
        }
        else {
            close(this->listen_sock);
            this->delete_pid_file();
            this->write_to_logs("SERVER STOPPED!", INFO);
        }

        exit(0);
    }

    this->write_to_logs("ERROR! SERVER NOT STOPPED!", ERROR);

    return false;
}

bool Server::server_reload(action_level_t level) {
    this->settings = MainServerSettings(this->config_filename);
    this->count_workflows = this->settings.get_count_workflows();
    this->server = this->settings.get_server();

    if (!this->daemonize(RELOAD_SERVER)) {
        this->write_to_logs("ERROR DAEMONIZE", ERROR);
        return false;
    }

    if (getpid() != this->old_master_process) {
        if (level == HARD_LEVEL) {
            this->write_to_logs("HARD SERVER RELOADING...New master process successfully started PID " +
                std::to_string(getpid()),
                WARNING);
        }
        else if (level == SOFT_LEVEL) {
            this->write_to_logs("SOFT SERVER RELOADING...New master process successfully started PID " +
                std::to_string(getpid()),
                WARNING);
        }
    }
    else {
        if (level == HARD_LEVEL) {
            process_hard_reload = NEW_MASTER;
        }
        else if (level == SOFT_LEVEL) {
            process_soft_reload = NEW_MASTER;
        }
        return true;
    }

    if (!apply_config(level)) {
        this->write_to_logs("ERROR APPLY CONFIG", ERROR);
        return false;
    }

    int status;
    if (level == HARD_LEVEL) {
        kill(this->old_master_process, SIGINT);
    }
    else if (level == SOFT_LEVEL) {
        kill(this->old_master_process, SIGHUP);
    }
    this->write_to_logs("Send kill to old master process", WARNING);

    while (!has_old_master_stopped)
        ;
    has_old_master_stopped = 0;

    this->write_to_logs("OLD MASTER FINISHED ALL CONNECTIONS WITH STATUS: " +
        std::to_string(WEXITSTATUS(status)) + " PID " + std::to_string(getpid()),
        INFO);

    this->fill_pid_file();

    this->old_master_process = this->new_master_process;
    this->new_master_process = 0;

    this->write_to_logs("SERVER RELOADED!", INFO);

    if (level == HARD_LEVEL) {
        process_hard_reload = EMPTY_MASTER;
    }
    else if (level == SOFT_LEVEL) {
        process_soft_reload = EMPTY_MASTER;
    }

    return true;
}

bool Server::apply_config(action_level_t level) {
    if (level == HARD_LEVEL) {
        this->write_to_logs("HARD SERVER RELOADING...NEW CONFIG APPLYING", WARNING);
    }
    else if (level == SOFT_LEVEL) {
        this->write_to_logs("SOFT SERVER RELOADING...NEW CONFIG APPLYING", WARNING);
    }

    this->count_workflows = this->settings.get_count_workflows();

    if (count_workflows <= 0) {
        this->write_to_logs("COUNT WORK PROCESSES MUST BE MORE 0", ERROR);
        return false;
    }

    if (!this->add_work_processes()) {
        this->write_to_logs("ERROR ADD WORKERS", ERROR);
        return false;
    }
    if (!this->fill_pid_file()) {
        this->write_to_logs("ERROR FILL PID FILE", ERROR);
        return false;
    }

    this->write_to_logs("COUNT WORK PROCESSES WAS SUCCESSFULLY CHECKED", WARNING);
    this->write_to_logs("Worker processes (" +
        std::to_string(this->workers_pid.size()) + ") successfully started",
        INFO);

    return true;
}

bool Server::bind_listen_sock() {
    this->listen_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (this->listen_sock == -1) {
        return false;
    }

    int enable = 1;
    if (setsockopt(this->listen_sock, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable)) == -1) {
        return false;
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(this->server.get_port());
    if (inet_aton(this->server.get_servername().c_str(), &addr.sin_addr) == -1) {
        return false;
    }

    if (bind(this->listen_sock, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        return false;
    }

    if (listen(this->listen_sock, BACKLOG) == -1) {
        return false;
    }

    return true;
}
