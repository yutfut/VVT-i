#include <request_handler_not_auth.h>
#include <http_status_code.h>
#include <data_base.h>
#include <fs_worker.h>
#include <fstream>
#include <utility>
#include <string>
#include <http_headers.h>

static std::string file_to_string(std::ifstream &&file) {
    if (!file.is_open()) {
        return {};
    }
    file.seekg(0, std::ios::end);
    size_t file_size = file.tellg();
    std::string str;
    str.resize(file_size);
    file.seekg(0);
    file.read(&str[0], static_cast<std::streamsize>( file_size));
    return str;
}

RequestHandlerNotAuth::RequestHandlerNotAuth(std::vector<Log *> &vector_logs) : IRequestHandler(vector_logs) {}


void RequestHandlerNotAuth::handle_request(HttpRequest &request, HttpResponse &response, FsWorker &fs_worker,
                                           DataBase &db_worker) {
    auto &headers = request.get_headers();
    if (headers[http_headers::command] == http_commands::download) {
        download_file_from_server(headers[http_headers::key], headers[http_headers::password], response, fs_worker,
                                  db_worker);
        return;
    }
    if (headers[http_headers::command] == http_commands::upload) {
        upload_file_to_server(headers[http_headers::filename], headers[http_headers::password], request.get_body(),
                              response, fs_worker,
                              db_worker);
        return;
    }
    response = create_response(HttpStatusCode::BadRequest, {});
}

bool
RequestHandlerNotAuth::download_file_from_server(const std::string &file_id, const std::string &opt_password,
                                                 HttpResponse &response,
                                                 FsWorker &fs_worker, DataBase &db_worker) {

    unauth_file_data_t upload_data;
    try {
        upload_data = db_worker.not_auth_mode.get_upload_file_date(file_id, opt_password);

        if (upload_data.filename.empty()) {
            response = create_response(HttpStatusCode::Forbidden, {{http_headers::command, http_commands::download}});
            return false;
        }
    } catch (const std::string &error_msg) {
        write_to_logs(error_msg, ERROR);
        response = HttpResponse({{http_headers::content_length, "0"}}, {}, 1, 1, HttpStatusCode::InternalServerError,
                                get_message(HttpStatusCode::InternalServerError));
        return false;
    }

    std::string file{
            file_to_string(fs_worker.not_auth_usr.get_file(std::filesystem::path(file_id),
                                                           std::filesystem::path(upload_data.upload_date)))};

    if (fs_worker.not_auth_usr.err_code) {
        write_to_logs(fs_worker.not_auth_usr.err_code.message(), ERROR);
        response = create_response(HttpStatusCode::InternalServerError,
                                   {{http_headers::command, http_commands::download}});
        return false;
    }
    response = create_response(HttpStatusCode::OK, {{http_headers::filename, upload_data.filename},
                                                    {http_headers::command,  http_commands::download}},
                               std::move(file));
    return true;
}

bool RequestHandlerNotAuth::upload_file_to_server(const std::string &filename, const std::string &opt_pswd,
                                                  const std::string &file_content, HttpResponse &response,
                                                  FsWorker &fs_worker, DataBase &db_worker) {
    unauth_file_data_t file_data;
    try {
        file_data = db_worker.not_auth_mode.add_file(filename, opt_pswd);

        if (file_data.uuid.empty()) {
            response = create_response(HttpStatusCode::Forbidden, {{http_headers::command, http_commands::upload}});
            return false;
        }
    } catch (const std::string &error_msg) {
        write_to_logs(error_msg, ERROR);
        response = create_response(HttpStatusCode::InternalServerError,
                                   {{http_headers::command, http_commands::upload}});
        return false;
    }

    if (!fs_worker.not_auth_usr.write_to_file(file_content, file_data.uuid, file_data.upload_date)) {
        db_worker.not_auth_mode.delete_file(file_data.uuid);
        write_to_logs(fs_worker.not_auth_usr.err_code.message(), ERROR);
        response = create_response(HttpStatusCode::InternalServerError,
                                   {{http_headers::command, http_commands::upload}});
        return false;
    }

    response = create_response(HttpStatusCode::OK, {{http_headers::key,     file_data.uuid},
                                                    {http_headers::command, http_commands::upload}}, {});
    return true;
}
