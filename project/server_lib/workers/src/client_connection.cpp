#include "client_connection.h"
#include "http_status_code.h"
#include <request_handler_not_auth.h>
#include <http_status_code.h>
#include <mailio/message.hpp>
#include <mailio/smtp.hpp>
#include <fmt/core.h>
#include <regex>

#define CLIENT_SEC_TIMEOUT 180
#define LENGTH_LINE_FOR_RESERVE 256


ClientConnection::ClientConnection(class ServerSettings *server_settings,
                                   std::vector<Log *> &vector_logs, FsWorker &fs_worker,
                                   DataBase &db_worker) : server_settings(server_settings),
                                                          vector_logs(vector_logs),
                                                          start_connection(clock()), fs_worker(fs_worker),
                                                          db_worker(db_worker) {
}

void ClientConnection::set_socket(int socket) {
    this->sock = socket;
}

connection_status_t ClientConnection::connection_processing() {
    if (this->stage == ERROR_STAGE) {
        return ERROR_WHILE_CONNECTION_PROCESSING;
    }

    if (this->stage == GET_REQUEST) {
        bool is_succeeded;
        try {
            is_succeeded = this->get_request();
        }
        catch (std::exception &e) {
            this->stage = BAD_REQUEST;
            this->message_to_log(ERROR_BAD_REQUEST);
            return ERROR_IN_REQUEST;
        }
        if (is_succeeded) {
            this->stage = HANDLE_REQUEST;
        } else if (this->is_timeout()) {
            this->message_to_log(ERROR_TIMEOUT);
            return CONNECTION_TIMEOUT_ERROR;
        }
    }

    if (this->stage == HANDLE_REQUEST) {
        if (this->handle_request()) {
            this->stage = SEND_RESPONSE;
        }
    }

    if (this->stage == SEND_RESPONSE) {
        if (this->send_response()) {
            this->message_to_log(INFO_CONNECTION_FINISHED);
            return CONNECTION_FINISHED;
        } else if (this->is_timeout()) {
            this->message_to_log(ERROR_TIMEOUT);
            return CONNECTION_TIMEOUT_ERROR;
        }
    }

    return CONNECTION_PROCESSING;
}

bool ClientConnection::get_request() {
    bool has_read_data = false;
    char last_char;
    std::string line;

    line.reserve(LENGTH_LINE_FOR_RESERVE);
    while (read(this->sock, &last_char, sizeof(char)) == sizeof(char)) {
        line.push_back(last_char);
        if (last_char == '\n') {
            this->request.add_line(line);
            line.clear();
            line.reserve(LENGTH_LINE_FOR_RESERVE);
        }
        has_read_data = true;
    }

    if (this->request.requst_ended()) {
        return true;
    }

    if (has_read_data) {
        this->timeout = clock();
    }
    return false;
}

bool ClientConnection::handle_request() {
    write_to_logs("-----------------THE REQUEST---------------", ERROR);
    std::for_each(request.get_headers().begin(), request.get_headers().end(),
                  [this](const auto &el) { write_to_logs(el.first + ": " + el.second, ERROR); });
    write_to_logs(request.get_body(), ERROR);
    write_to_logs("----------------END OF REQUEST---------------", ERROR);

    RequestHandlerNotAuth handler(vector_logs);
    handler.handle_request(request, response, fs_worker, db_worker);

    write_to_logs("----------------THE ANSWER---------------", ERROR);
    std::for_each(response.get_headers().begin(), response.get_headers().end(),
                  [this](const auto &el) { write_to_logs(el.first + ": " + el.second, ERROR); });
    write_to_logs(response.get_body(), ERROR);

    write_to_logs("----------------END OF ANSWER---------------", ERROR);

    write_to_logs(this->response.get_headers()["status"], ERROR);

    if (this->response.get_headers()["status"] != "200") {
        return true;
    }

    this->send_message_on_email(0);
    return true;
}

bool ClientConnection::send_message_on_email(size_t step) {
    if (step > 1) {
        return false;
    }

    write_to_logs("12345", ERROR);

    try
    {
        mailio::message message;

        message.from(mailio::mail_address("VVTI", "vvticlient@gmail.com"));
        message.add_recipient(mailio::mail_address("User", this->request.get_headers()["email"]));

        message.content_type(mailio::message::media_type_t::TEXT, "html", "utf-8");

        std::ifstream file;

        if (this->request.get_headers()["command"] == "upload") {
            message.subject("Your code");

            if (this->request.get_headers().find("content-length") != this->request.get_headers().end() &&
                !this->request.get_headers()["jwt"].empty()) {
                file.open("./static/auth_code.html");
            } else {
                file.open("./static/not_auth_code.html");
            }
        } else if (this->request.get_headers()["command"] == "register") {
            message.subject("Successfull registration");
            file.open("./static/registration.html");
        }

        std::string html;

        std::string line;
        while(std::getline(file, line))
        {
            html += line;
        }
        file.close();

        if (this->request.get_headers()["command"] == "upload") {
            if (this->request.get_headers().find("content-length") != this->request.get_headers().end() &&
                !this->request.get_headers()["jwt"].empty()) {
                html = fmt::format(html, this->request.get_headers()["filename"]);
                html = std::regex_replace(html, std::regex("  "), "\r\n");
            } else {
                html = fmt::format(
                    html,
                    this->request.get_headers()["filename"],
                    this->response.get_headers()["key"]
                );
            }
        }

        message.content(html);

        mailio::smtps conn("smtp.gmail.com", 587);

        conn.authenticate("vvticlient@gmail.com", "ycfjgpxboqpegxrx", mailio::smtps::auth_method_t::START_TLS);
        conn.submit(message);
    }
    catch (mailio::smtp_error& exc)
    {
        return send_message_on_email(step + 1);
    }
    catch (mailio::dialog_error& exc)
    {
        return send_message_on_email(step + 1);
    }

    return true;
}

bool ClientConnection::send_response() {
    bool has_written_data = false;

    std::string response_str = this->response.get_string();

    while (write(this->sock, response_str.c_str() + res_pos, sizeof(char)) == sizeof(char)) {
        ++res_pos;
        if (res_pos == response_str.size() - 1) {
            return true;
        }
        has_written_data = true;
    }

    if (has_written_data) {
        this->timeout = clock();
    }

    return false;
}

void ClientConnection::message_to_log(log_messages_t log_type) {
    switch (log_type) {
        case INFO_CONNECTION_FINISHED: {
            int status = this->response.get_status();
            if (status % 100 == 4) {
                this->write_to_logs("Connection [" + this->request.get_method() + "] [URL " + this->request.get_url() +
                                    "] [STATUS " + std::to_string(status) +
                                    "] [WRK PID " + std::to_string(getpid()) + "] [CLIENT SOCKET " +
                                    std::to_string(this->sock) + "] [TIME " +
                                    std::to_string((clock() - start_connection) / (double) CLOCKS_PER_SEC * 1000) +
                                    " ms]",
                                    ERROR);
            } else {
                this->write_to_logs("Connection [" + this->request.get_method() + "] [URL " + this->request.get_url() +
                                    "] [STATUS " + std::to_string(status) +
                                    "] [WRK PID " + std::to_string(getpid()) + "] [CLIENT SOCKET " +
                                    std::to_string(this->sock) + "] [TIME " +
                                    std::to_string((clock() - start_connection) / (double) CLOCKS_PER_SEC * 1000) +
                                    " ms]",
                                    INFO);
            }
            break;
        }
        case ERROR_READING_REQUEST:
            this->write_to_logs("Reading request error [WORKER PID " + std::to_string(getpid()) + "] [CLIENT SOCKET " +
                                std::to_string(this->sock) + "]", ERROR);
            break;
        case ERROR_SENDIND_RESPONSE:
            this->write_to_logs("Sending response error [WORKER PID " + std::to_string(getpid()) + "] [CLIENT SOCKET " +
                                std::to_string(this->sock) + "]", ERROR);
            break;
        case ERROR_BAD_REQUEST:
            this->write_to_logs("Bad request error [WORKER PID " + std::to_string(getpid()) + "] [CLIENT SOCKET " +
                                std::to_string(this->sock) + "]", ERROR);
            break;
        case ERROR_TIMEOUT:
            this->write_to_logs("Timeout error [WORKER PID " + std::to_string(getpid()) + "] [CLIENT SOCKET " +
                                std::to_string(this->sock) + "]", ERROR);
            break;
    }
}

void ClientConnection::write_to_logs(std::string message, bl::trivial::severity_level lvl) {
    for (auto &vector_log: this->vector_logs) {
        vector_log->log(message, lvl);
    }
}

int ClientConnection::get_socket() {
    return this->sock;
}

bool ClientConnection::is_timeout() {
    return (clock() - this->timeout) / (double) CLOCKS_PER_SEC > CLIENT_SEC_TIMEOUT;
}
