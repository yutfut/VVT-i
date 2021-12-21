#include <request_handler_not_auth.h>
#include <http_status_code.h>
#include <data_base.h>
#include <fs_worker.h>
#include <fstream>
#include <utility>
#include <string>

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

RequestHandlerNotAuth::RequestHandlerNotAuth(std::vector<Log *> &vector_logs) : vector_logs(vector_logs) {}

HttpResponse
RequestHandlerNotAuth::create_response(HttpStatusCode status, std::map <std::string, std::string> &&additional_headers,
                                       std::string &&body) {
    additional_headers.merge(std::map < std::string, std::string > {{"content-length", std::to_string(body.size())},
                                                                    {"status",         std::to_string(status)},
                                                                    {"message",        get_message(status)}});
    return {std::move(additional_headers), std::move(body), 1, 1, status, get_message(status)};
}

void RequestHandlerNotAuth::write_to_logs(std::string message, bl::trivial::severity_level lvl) {
    for (auto &vector_log: this->vector_logs) {
        vector_log->log(message, lvl);
    }
}

void RequestHandlerNotAuth::handle_request(HttpRequest &request, HttpResponse &response, FsWorker &fs_worker,
                                           DataBase &db_worker) {
    auto &headers = request.get_headers();
    write_to_logs("UNAUTH_HANDLER", ERROR);
    if (headers["command"] == "download") {
        download_file_from_server(headers["key"], headers["password"], response, fs_worker, db_worker);
        return;
    }
    if (headers["command"] == "upload") {
        upload_file_to_server(headers["filename"], headers["password"], request.get_body(), response, fs_worker,
                              db_worker);
        return;
    }
    response = create_response(HttpStatusCode::BadRequest, {});
    return;
}

bool
RequestHandlerNotAuth::download_file_from_server(const std::string &file_id, const std::string &opt_pswd,
                                                 HttpResponse &response,
                                                 FsWorker &fs_worker, DataBase &db_worker) {

    unauth_file_data_t upload_data;
    try {
        write_to_logs("__LINE__", ERROR);
        write_to_logs(std::to_string(__LINE__), ERROR);
        upload_data = db_worker.not_auth_mode.get_upload_file_date(file_id, opt_pswd);
        write_to_logs("__LINE__", ERROR);
        write_to_logs(std::to_string(__LINE__), ERROR);

        if (upload_data.filename.empty()) {
            response = create_response(HttpStatusCode::Forbidden, {{"command", "download"}});
            return false;
        }
    } catch (const std::string &error_msg) {
        write_to_logs(error_msg, ERROR);
        response = HttpResponse({{"content-length", "0"}}, {}, 1, 1, HttpStatusCode::InternalServerError,
                                get_message(HttpStatusCode::InternalServerError));
        return false;
    }

    std::string file{
            file_to_string(fs_worker.not_auth_usr.get_file(fs::path(file_id), fs::path(upload_data.upload_date)))};

    if (fs_worker.not_auth_usr.err_code) {
        write_to_logs(fs_worker.not_auth_usr.err_code.message(), ERROR);
        response = create_response(HttpStatusCode::InternalServerError, {{"command", "download"}});
        return false;
    }
    response = create_response(HttpStatusCode::OK, {{"filename", upload_data.filename},
                                                    {"command",  "download"}}, std::move(file));
    return true;
}

bool RequestHandlerNotAuth::upload_file_to_server(const std::string &filename, const std::string &opt_pswd,
                                                  const std::string &file, HttpResponse &response,
                                                  FsWorker &fs_worker, DataBase &db_worker) {
    unauth_file_data_t file_data;
    try {
        write_to_logs(std::to_string(__LINE__), ERROR);
        write_to_logs("I GO TO add_unauth_user_file()", ERROR);
        file_data = db_worker.not_auth_mode.add_file(filename, opt_pswd);
        write_to_logs("I LEAVE add_unauth_user_file()", ERROR);
        write_to_logs(std::to_string(__LINE__), ERROR);
        if (file_data.uuid.empty()) {
            response = create_response(HttpStatusCode::Forbidden, {{"command", "upload"}});
            return false;
        }
    } catch (const std::string &error_msg) {
        write_to_logs(error_msg, ERROR);
        response = create_response(HttpStatusCode::InternalServerError, {{"command", "upload"}});
        return false;
    }

    fs::path tmpfile_path = fs_worker.not_auth_usr.get_root() / "tmp" / file_data.uuid;
    std::ofstream tmpfile{tmpfile_path, std::ios_base::binary};
//    write_to_logs(std::to_string(__LINE__), ERROR);
//    write_to_logs(tmpfile_path, ERROR);
//    write_to_logs(std::to_string(file.size()), ERROR);
//    write_to_logs(std::to_string(__LINE__), ERROR);
//    write_to_logs(file, ERROR);
    tmpfile << file;
    tmpfile.close();
    write_to_logs(tmpfile_path,ERROR);
    write_to_logs( static_cast<fs::path>(file_data.uuid) / static_cast<fs::path>(file_data.upload_date) , ERROR);

    if (!fs_worker.not_auth_usr.move_file_to_fs(tmpfile_path, file_data.uuid, file_data.upload_date)) {
        db_worker.not_auth_mode.delete_certain_file(file_data.uuid);
        write_to_logs(fs_worker.not_auth_usr.err_code.message(), ERROR);
        response = create_response(HttpStatusCode::InternalServerError, {{"command", "upload"}});
        return false;
    }

    response = create_response(HttpStatusCode::OK, {{"key",     file_data.uuid},
                                                    {"command", "upload"}}, {});
    return true;
}
