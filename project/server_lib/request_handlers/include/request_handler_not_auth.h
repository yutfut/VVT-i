#pragma once

#include <http_request.h>
#include <http_response.h>
#include <http_status_code.h>
#include <fs_worker.h>
#include <data_base.h>
#include <log.h>
#include <irequest_handler.h>

class RequestHandlerNotAuth: IRequestHandler {
public:
    explicit RequestHandlerNotAuth(std::vector<Log *> &vector_logs);

    void
    handle_request(HttpRequest &request, HttpResponse &response, FsWorker &fs_worker, DataBase &db_worker) override;

private:
    bool download_file_from_server(const std::string &file_id, const std::string &opt_password, HttpResponse &response,
                                   FsWorker &fs_worker, DataBase &db_worker);

    bool
    upload_file_to_server(const std::string &filename, const std::string &opt_pswd, const std::string &file_content,
                          HttpResponse &response, FsWorker &fs_worker, DataBase &db_worker);
};
