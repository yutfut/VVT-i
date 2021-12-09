#pragma once

#include <http_request.h>
#include <http_response.h>
#include <http_status_code.h>
#include <fs_worker.h>
#include <data_base.h>
#include <log.h>

class RequestHandlerNotAuth {
public:
    RequestHandlerNotAuth(std::vector<Log *> &vector_logs);

    bool
    handle_request(HttpRequest &request, HttpResponse &response, FsWorker &fs_worker, DataBase &db_worker);

private:
    bool download_file_from_server(const std::string &file_id, const std::string &opt_pswd, HttpResponse &response,
                                   FsWorker &fs_worker, DataBase &db_worker);

    bool
    upload_file_to_server(const std::string &filename, const std::string &opt_pswd, const std::string &file,
                          HttpResponse &response, FsWorker &fs_worker, DataBase &db_worker);

    HttpResponse create_response(HttpStatusCode status, std::map<std::string, std::string> &&additional_headers = {},
                                 std::string &&body = {});

    void write_to_logs(std::string message, bl::trivial::severity_level lvl);

    std::vector<Log *> &vector_logs;
};
