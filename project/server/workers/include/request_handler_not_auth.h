#pragma once

#include <http_request.h>
#include <http_response.h>
#include <fs_worker.h>
#include <data_base.h>

class RequestHandlerNotAuth {
public:
    bool
    handle_request(HttpRequest &request, HttpResponse &response,  FsWorker &fs_worker,  DataBase &db_worker);

private:
    bool download_file_from_server(const std::string &file_id, const std::string &opt_pswd, HttpResponse &response,
                                    FsWorker &fs_worker,  DataBase &db_worker);

    bool
    upload_file_to_server(const std::string &filename, const std::string &opt_pswd, const std::string &file, HttpResponse &response,  FsWorker &fs_worker,
                           DataBase &db_worker);;
};
