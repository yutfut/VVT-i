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
    str.resize(file_size);
    file.seekg(0);
    file.read(&str[0], static_cast<std::streamsize>( file_size));
    return str;
}

// TODO: добавить метод в класс
HttpResponse
RequestHandlerNotAuth::create_response(HttpStatusCode status, std::map<std::string, std::string> &&additional_headers,
                                       std::string &&body) {
    additional_headers.merge(std::map<std::string, std::string>{{"content-length", std::to_string(body.size())},
                                                                {"status",         std::to_string(status)},
                                                                {"message",        get_message(status)}});
    return {std::move(additional_headers), std::move(body), 1, 1, status, get_message(status)};
}


RequestHandlerNotAuth::RequestHandlerNotAuth(std::vector<Log *> &vector_logs) : vector_logs(vector_logs) {}


void RequestHandlerNotAuth::write_to_logs(std::string message, bl::trivial::severity_level lvl) {
    for (auto &vector_log: this->vector_logs) {
        vector_log->log(message, lvl);
    }
}

bool RequestHandlerNotAuth::handle_request(HttpRequest &request, HttpResponse &response, FsWorker &fs_worker,
                                           DataBase &db_worker) {
    auto &headers = request.get_headers();
    if (headers["command"] == "download") {
        return RequestHandlerNotAuth::download_file_from_server(headers["key"], headers["password"],
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

    write_to_logs("I AM IN DLD", ERROR);
    std::string upload_date;
    try {
        upload_date = db_worker.not_auth_mode.get_upload_file_date(file_id, opt_pswd);

        write_to_logs(std::to_string(__LINE__), ERROR);
        if (upload_date.empty()) {
            response = create_response(HttpStatusCode::Forbidden, {{"command", "download"}});
            write_to_logs(std::to_string(__LINE__), ERROR);
            return true;
        }
    } catch (const std::string &error_msg) {
        write_to_logs(std::to_string(__LINE__), ERROR);
        //TODO: вывод в лог error_msg

        response = HttpResponse({{"content-length", "0"}}, {}, 1, 1, HttpStatusCode::InternalServerError,
                                get_message(HttpStatusCode::InternalServerError));
        //TODO: а в каких случаях кидаетсся исключение? какой ответ правильно отдавать?
        return true;
    }

    write_to_logs(std::to_string(__LINE__), ERROR);
    std::string file{file_to_string(fs_worker.not_auth_usr.get_file(fs::path(file_id), fs::path(upload_date)))};

    write_to_logs("FILE", ERROR);
    write_to_logs(file, ERROR);

    write_to_logs(std::to_string(__LINE__), ERROR);
    if (fs_worker.not_auth_usr.err_code) {
        write_to_logs(fs_worker.not_auth_usr.err_code.message(), ERROR);
        response = create_response(HttpStatusCode::InternalServerError, {{"command", "download"}});
        return true;
    }
    write_to_logs(std::to_string(__LINE__), ERROR);
// TODO: убрать hardcode filename, нужно получить данные из БД
    response = create_response(HttpStatusCode::OK, {{"filename", "filename"}, {"command", "download"}}, std::move(file));
    return true;
}

bool RequestHandlerNotAuth::upload_file_to_server(const std::string &filename, const std::string &opt_pswd,
                                                  const std::string &file, HttpResponse &response,
                                                  FsWorker &fs_worker, DataBase &db_worker) {
    unauth_file_data_t file_data;
    write_to_logs("I AM IN UPLOAD_FILE...", ERROR);
    write_to_logs(std::to_string(__LINE__), ERROR);
    try {
        write_to_logs(std::to_string(__LINE__), ERROR);
        file_data = db_worker.not_auth_mode.add_unauth_user_file(filename, opt_pswd);
        write_to_logs(std::to_string(__LINE__), ERROR);
        if (file_data.uuid.empty()) {
            write_to_logs(std::to_string(__LINE__), ERROR);
            response = create_response(HttpStatusCode::Forbidden, {{"command", "upload"}});

            write_to_logs(std::to_string(__LINE__), ERROR);
            return true;
        }
    } catch (const std::string &error_msg) {
        //TODO: вывод в лог error_msg

        write_to_logs(std::to_string(__LINE__), ERROR);
        response = create_response(HttpStatusCode::InternalServerError, {{"command", "upload"}});
        //TODO: а в каких случаях кидается исключение? какой ответ правильно отдавать?
        return true;
    }

    write_to_logs(std::to_string(__LINE__), ERROR);
    fs::path tmpfile_path = fs_worker.not_auth_usr.get_root() / "tmp" /
                            file_data.uuid; //TODO:  сделать по-нормальному в нормальную папку тмп
    std::ofstream tmpfile{tmpfile_path};
    write_to_logs(tmpfile_path, ERROR);
    if (tmpfile.is_open()) {
        write_to_logs("TMPFILE IS OPEN", ERROR);
    }
    tmpfile << file;
    if (tmpfile.bad()) {
        write_to_logs("TMPFILE BAD", ERROR);
    }

    tmpfile.close();
    if (!fs_worker.not_auth_usr.move_file_to_fs(tmpfile_path, file_data.uuid, file_data.upload_data)) {
        // TODO: db_worker.not_auth_mode.delete_certain_file()
        // TODO: вывод в лог err_code.message()
        write_to_logs("ARGS", ERROR);
        write_to_logs(tmpfile_path, ERROR);
        write_to_logs(file_data.uuid, ERROR);
        write_to_logs(file_data.upload_data, ERROR);

        write_to_logs(fs_worker.not_auth_usr.get_root() / file_data.upload_data / file_data.uuid, ERROR);

        write_to_logs("ERROR_CODE", ERROR);
        write_to_logs(fs_worker.not_auth_usr.err_code.message(), ERROR);
        write_to_logs(std::to_string(__LINE__), ERROR);
        response = create_response(HttpStatusCode::InternalServerError, {{"command", "upload"}});
        return true;
    }

    write_to_logs(std::to_string(__LINE__), ERROR);
    response = create_response(HttpStatusCode::OK, {{"key", file_data.uuid},{"command", "upload"}}, {});
    return true;
}
