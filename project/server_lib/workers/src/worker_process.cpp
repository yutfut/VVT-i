#include "worker_process.h"

#define EPOLL_SIZE 2048
#define EPOLL_RUN_TIMEOUT 1

bool is_hard_stop = false;
bool is_soft_stop = false;

WorkerProcess::WorkerProcess(int listen_sock, class ServerSettings *server_settings,
                             std::vector<Log *> &vector_logs, const std::filesystem::path &fs_root_path, const database_t &db_config)
        : listen_sock(listen_sock), server_settings(server_settings), vector_logs(vector_logs), fs_worker(fs_root_path),
          db_worker({db_config.user, db_config.password, db_config.host,std::to_string(db_config.port), db_config.dbname}) {
    signal(SIGPIPE, SIG_IGN);
    this->setup_signals();
}

void WorkerProcess::run() {
    static struct epoll_event ev, events[EPOLL_SIZE];
    ev.events = EPOLLIN;

    int epoll_fd = epoll_create(EPOLL_SIZE);
    ev.data.fd = this->listen_sock;
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, this->listen_sock, &ev);

    int client, epoll_events_count;

    while (!is_hard_stop && !is_soft_stop) {
        epoll_events_count = epoll_wait(epoll_fd, events, EPOLL_SIZE, EPOLL_RUN_TIMEOUT);
        for (int i = 0; i < epoll_events_count; ++i) {
            if (is_soft_stop || is_hard_stop) {
                break;
            }

            if (events[i].data.fd == this->listen_sock) {
                auto *client_connection = new(std::nothrow) ClientConnection(this->server_settings,
                                                                             this->vector_logs, fs_worker, db_worker);
                if (!client_connection) {
                    continue;
                }

                client = accept(this->listen_sock, NULL, NULL);
                if (client == -1) {
                    continue;
                }
                fcntl(client, F_SETFL, fcntl(client, F_GETFL, 0) | O_NONBLOCK);
                client_connection->set_socket(client);
                ev.events = EPOLLIN | EPOLLET;
                ev.data.ptr = (ClientConnection *) client_connection;

                epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client, &ev);
            } else {
                auto *client_connection = (ClientConnection *) events[i].data.ptr;
                connection_status_t connection_status = client_connection->connection_processing();
                if (connection_status == CONNECTION_FINISHED || connection_status == CONNECTION_TIMEOUT_ERROR ||
                    connection_status == ERROR_WHILE_CONNECTION_PROCESSING || connection_status == ERROR_IN_REQUEST) {
                    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, client_connection->get_socket(), &events[i]);
                    close(client_connection->get_socket());
                    delete client_connection;
                }
            }
        }
    }

    if (is_soft_stop) {
        this->message_to_log(INFO_SOFT_STOP_START);

        ev.data.fd = this->listen_sock;
        epoll_ctl(epoll_fd, EPOLL_CTL_DEL, this->listen_sock, &ev);

        epoll_events_count = epoll_wait(epoll_fd, events, EPOLL_SIZE, 0);
        for (int i = 0; i < epoll_events_count; ++i) {
            auto *client_connection = (ClientConnection *) events[i].data.ptr;
            connection_status_t connection_status = client_connection->connection_processing();
            if (connection_status == CONNECTION_FINISHED || connection_status == CONNECTION_TIMEOUT_ERROR ||
                connection_status == ERROR_WHILE_CONNECTION_PROCESSING) {
                epoll_ctl(epoll_fd, EPOLL_CTL_DEL, client_connection->get_socket(), &events[i]);
                close(client_connection->get_socket());
                delete client_connection;
            }
        }

        this->message_to_log(INFO_SOFT_STOP_DONE);
    } else {
        this->message_to_log(INFO_HARD_STOP_DONE);
    }
    exit(0);
}

void WorkerProcess::sighup_handler(int sig) {
    is_soft_stop = true;
}

void WorkerProcess::sigint_handler(int sig) {
    is_hard_stop = true;
}

void WorkerProcess::setup_signals() {
    struct sigaction act;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;

    act.sa_handler = sighup_handler;
    sigaction(SIGHUP, &act, nullptr);

    act.sa_handler = sigint_handler;
    sigaction(SIGINT, &act, nullptr);
}

void WorkerProcess::message_to_log(log_messages_t log_type) {
    switch (log_type) {
        case INFO_HARD_STOP_DONE:
            this->write_to_logs("HARD STOP for worker DONE [WORKER PID " + std::to_string(getpid()) + "]", INFO);
            break;
        case INFO_SOFT_STOP_START:
            this->write_to_logs("SOFT STOP for worker START [WORKER PID " + std::to_string(getpid()) + "]", INFO);
            break;
        case INFO_SOFT_STOP_DONE:
            this->write_to_logs("SOFT STOP for worker DONE [WORKER PID " + std::to_string(getpid()) + "]", INFO);
            break;
    }
}

void WorkerProcess::write_to_logs(const std::string &message, bl::trivial::severity_level lvl) {
    for (auto &vector_log: this->vector_logs) {
        vector_log->log(message, lvl);
    }
}
