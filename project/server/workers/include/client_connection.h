#pragma once

#include <ctime>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <exception>
#include <fcntl.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>

#include <fs_worker.h>
#include <data_base.h>

#include "http_request.h"
#include "http_response.h"
#include "http_date.h"
#include "server_settings.h"
#include "log.h"

typedef enum {
    CONNECTION_PROCESSING,
    CONNECTION_TIMEOUT_ERROR,
    CONNECTION_FINISHED,
    ERROR_WHILE_CONNECTION_PROCESSING,
    ERROR_IN_REQUEST,
} connection_status_t;

class ClientConnection {
public:
    ClientConnection() = default;

    ClientConnection(class ServerSettings *server_settings, std::vector<Log *> &vector_logs,  FsWorker &fs_worker,
                      DataBase &db_worker);

    connection_status_t connection_processing();

    int get_socket();

    void write_to_logs(std::string message, bl::trivial::severity_level lvl);

    void set_socket(int socket);

private:
    int sock;

    clock_t timeout, start_connection;

    std::vector<Log *> vector_logs;

    typedef enum {
        GET_REQUEST,
        HANDLE_REQUEST,
        SEND_RESPONSE,
        BAD_REQUEST,
        ERROR_STAGE
    } connection_stages_t;

    typedef enum {
        INFO_CONNECTION_FINISHED,
        ERROR_READING_REQUEST,
        ERROR_SENDIND_RESPONSE,
        ERROR_BAD_REQUEST,
        ERROR_TIMEOUT
    } log_messages_t;

    connection_stages_t stage = GET_REQUEST;

    class ServerSettings *server_settings;

    HttpRequest request;
    HttpResponse response;

    int res_pos = 0;

    // Return true if their connection processing stage is finished correctly
    bool get_request();

    bool handle_request();

    bool send_response();

    void message_to_log(log_messages_t log_type);

    bool is_timeout();

     FsWorker &fs_worker;

     DataBase &db_worker;
};
