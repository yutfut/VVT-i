#include <request_handler_not_auth.h>
#include <http_status_code.h>
#include <data_base.h>
#include <fs_worker.h>
#include <fstream>

std::string file_to_string(std::ifstream &&file) {
    if (!file.is_open()) {
        return {};
    }
    file.seekg(0, std::ios::end);
    size_t file_size = file.tellg();
    std::string str;
    str.reserve(file_size);
    file.seekg(0);
    file.read(&str[0], static_cast<std::streamsize>( file_size));
    return str;
}

HttpResponse create_response(HttpStatusCode status, std::string &&body) {
    return {{{"content-length", std::to_string(body.size())}, {"status", std::to_string(status)},
             {"message", get_message(status)}}, std::move(body), 1, 1,
            HttpStatusCode::Forbidden,get_message(HttpStatusCode::Forbidden)};
}

bool RequestHandlerNotAuth::handle_request(HttpRequest &request, HttpResponse &response, FsWorker &fs_worker,
                                           DataBase &db_worker) {
    auto &headers = request.get_headers();
    if (headers["command"] == "download") {
        return RequestHandlerNotAuth::download_file_from_server(headers["filename"], headers["password"],
                                                                response,
                                                                fs_worker, db_worker);
    }
    if (headers["command"] == "upload") {
        return RequestHandlerNotAuth::upload_file_to_server(headers["filename"],
                                                            headers["password"], request.get_body(),
                                                            response, fs_worker, db_worker);
    }
    response = create_response(HttpStatusCode::BadRequest, {});
    return true;
}

bool
RequestHandlerNotAuth::download_file_from_server(const std::string &file_id, const std::string &opt_pswd,
                                                 HttpResponse &response,
                                                 FsWorker &fs_worker, DataBase &db_worker) {
    std::string upload_date;
    try {
        upload_date = db_worker.not_auth_mode.get_upload_file_date(file_id, opt_pswd);
        if (upload_date.empty()) {
            response = create_response(HttpStatusCode::Forbidden, {});
            return true;
        }
    } catch (const std::string &error_msg) {
        //TODO: вывод в лог error_msg
        response = HttpResponse({{"content-length", "0"}}, {}, 1, 1, HttpStatusCode::InternalServerError,
                                get_message(HttpStatusCode::InternalServerError));
        //TODO: а в каких случаях кидаетсся исключение? какой ответ правильно отдавать?
        return true;
    }

    std::string file{file_to_string(fs_worker.not_auth_usr.get_file(fs::path(file_id), fs::path(upload_date)))};

    if (fs_worker.not_auth_usr.err_code) {
        response = create_response(HttpStatusCode::InternalServerError, {});
        //TODO: вывод в лог error_msg
        return true;
    }
    //TODO: заголовок filename

    response = create_response(HttpStatusCode::OK, std::move(file));
    return true;
}

bool RequestHandlerNotAuth::upload_file_to_server(const std::string &filename, const std::string &opt_pswd,
                                                  const std::string &file, HttpResponse &response,
                                                  FsWorker &fs_worker, DataBase &db_worker) {
    unauth_file_data_t file_data;
    try {
        file_data = db_worker.not_auth_mode.add_unauth_user_file(filename, opt_pswd);
        if (file_data.uuid.empty()) {
            response = create_response(HttpStatusCode::Forbidden, {});
            return true;
        }
    } catch (const std::string &error_msg) {
        //TODO: вывод в лог error_msg
        response = create_response(HttpStatusCode::InternalServerError, {});
        //TODO: а в каких случаях кидается исключение? какой ответ правильно отдавать?
        return true;
    }
    fs::path tmpfile_path = fs_worker.get_root_dir() / "tmp" / file_data.uuid;
    std::ofstream tmpfile{tmpfile_path};
    tmpfile << file;
    tmpfile.close();
    if (!fs_worker.not_auth_usr.move_file_to_fs(tmpfile_path, file_data.uuid, file_data.upload_data)) {
        // TODO: db_worker.not_auth_mode.delete_certain_file()
        // TODO: вывод в лог err_code.message()
        response = create_response(HttpStatusCode::InternalServerError, {});
        return true;
    }
    response = create_response(HttpStatusCode::OK, {});
    return true;
}




