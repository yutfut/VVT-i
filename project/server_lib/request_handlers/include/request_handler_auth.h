#pragma once

#include <http_request.h>
#include <http_response.h>
#include <http_status_code.h>
#include <fs_worker.h>
#include <data_base.h>
#include <log.h>
#include <irequest_handler.h>

class RequestHandlerAuth: IRequestHandler {
public:
    explicit RequestHandlerAuth(std::vector<Log *> &vector_logs);

    void
    handle_request(HttpRequest &request, HttpResponse &response, FsWorker &fs_worker, DataBase &db_worker) override;

private:
    void
    register_user(const std::string &email, const std::string &password, HttpResponse &response, FsWorker &fs_worker,
                  DataBase &db_worker);

    void
    login_user(const std::string &email, const std::string &password, HttpResponse &response, DataBase &db_worker);

    void get_user_dir_list_files(int id, const std::filesystem::path &work_dir, HttpResponse &response,
                                 DataBase &db_worker);


    void change_user_dir(int id, const std::filesystem::path &work_dir, HttpResponse &response, DataBase &db_worker);

    void make_user_subdir(int id, const std::filesystem::path &work_dir, HttpResponse &response, FsWorker &fs_worker,
                          DataBase &db_worker);

    void remove_user_subdir(int id, const std::filesystem::path &work_dir, HttpResponse &response, FsWorker &fs_worker,
                            DataBase &db_worker);

    static bool
    download_file_from_server(int id, const std::filesystem::path &work_dir, const std::filesystem::path &filename, HttpResponse &response,
                              FsWorker &fs_worker);

    void
    upload_file_to_server(int id, const std::filesystem::path &work_dir, const std::filesystem::path &filename, const std::string &file,
                          HttpResponse &response, FsWorker &fs_worker, DataBase &db_worker);

    static std::string create_jwt(int id);

    static int get_id_from_jwt(const std::string &jwt);

    static const std::string jwt_key;
    static const std::string jwt_algorithm;
};
