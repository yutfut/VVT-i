#pragma once

#include <http_request.h>
#include <http_response.h>
#include <fs_worker.h>
#include <data_base.h>

class RequestHandlerNotAuth {
public:
    bool
    handle_request(HttpRequest &request, HttpResponse &response, const FsWorker &fs_worker, const DataBase &db_worker);

private:
    bool download_file(const std::string &file_id, const std::string &opt_pswd, HttpResponse &response,
                       const FsWorker &fs_worker, const DataBase &db_worker);

    bool
    upload_file(const std::string &filename, const std::string &file, HttpResponse &response, const FsWorker &fs_worker,
                const DataBase &db_worker);;
};
