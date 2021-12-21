#pragma once

#include <http_request.h>
#include <http_response.h>
#include <http_status_code.h>
#include <fs_worker.h>
#include <data_base.h>
#include <log.h>

class RequestHandlerAuth
{
public:
    RequestHandlerAuth(std::vector<Log *> &vector_logs);

    void
    handle_request(HttpRequest &request, HttpResponse &response, FsWorker &fs_worker, DataBase &db_worker);

private:
    void
    register_user(const std::string &email, const std::string &password, HttpResponse &response, FsWorker &fs_worker,
                  DataBase &db_worker);

    void
    login_user(const std::string &email, const std::string &password, HttpResponse &response, FsWorker &fs_worker,
               DataBase &db_worker);

    void get_user_dir_list_files(int id, const std::string &curr_dir, const std::string &work_dir, HttpResponse &response,
                                 DataBase &db_worker);

    void change_user_dir(int id, const std::string &curr_dir, const std::string &work_dir, HttpResponse &response,
                         DataBase &db_worker);

    //    bool download_file_from_server(const std::string &file_id, const std::string &opt_pswd, HttpResponse &response,
    //                                   FsWorker &fs_worker, DataBase &db_worker);
    //
    //    bool
    //    upload_file_to_server(const std::string &filename, const std::string &opt_pswd, const std::string &file,
    //                          HttpResponse &response, FsWorker &fs_worker, DataBase &db_worker);

    HttpResponse create_response(HttpStatusCode status, std::map<std::string, std::string> &&additional_headers = {},
                                 std::string &&body = {});

    void write_to_logs(std::string message, bl::trivial::severity_level lvl);

    std::string create_jwt(int id);

    int get_id_from_jwt(const std::string &jwt);

    std::vector<Log *> &vector_logs;

    static const std::string jwt_key;
    static const std::string jwt_algorithm;
};
