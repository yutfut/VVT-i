#pragma once

#include <http_request.h>
#include <http_response.h>
#include <http_status_code.h>
#include <fs_worker.h>
#include <data_base.h>
#include <log.h>

class IRequestHandler {
protected:
    explicit IRequestHandler(std::vector<Log *> &vector_logs);
    virtual void
    handle_request(HttpRequest &request, HttpResponse &response, FsWorker &fs_worker, DataBase &db_worker) = 0;

    static HttpResponse
    create_response(HttpStatusCode status, std::map<std::string, std::string> &&additional_headers = {},
                    std::string &&body = {});

    void write_to_logs(const std::string &message, bl::trivial::severity_level lvl);

    std::vector<Log *> &vector_logs;
};
