#include <http_request.h>
#include <http_response.h>
#include <http_status_code.h>
#include <fs_worker.h>
#include <data_base.h>
#include <log.h>
#include <request_handler_auth.h>
#include <fstream>
#include <utility>
#include <string>
#include <jwt/jwt.hpp>

const std::string RequestHandlerAuth::jwt_key = "asWt5e^%6*7E";
const std::string RequestHandlerAuth::jwt_algorithm = "HS256";

RequestHandlerAuth::RequestHandlerAuth(std::vector<Log *> &vector_logs) : vector_logs(vector_logs) {}

HttpResponse
RequestHandlerAuth::create_response(HttpStatusCode status, std::map<std::string, std::string> &&additional_headers,
                                    std::string &&body) {
    additional_headers.merge(std::map<std::string, std::string>{{"content-length", std::to_string(body.size())},
                                                                {"status",         std::to_string(status)},
                                                                {"message",        get_message(status)}});
    return {std::move(additional_headers), std::move(body), 1, 1, status, get_message(status)};
}

void RequestHandlerAuth::write_to_logs(std::string message, bl::trivial::severity_level lvl) {
    for (auto &vector_log: this->vector_logs) {
        vector_log->log(message, lvl);
    }
}

void RequestHandlerAuth::handle_request(HttpRequest &request, HttpResponse &response, FsWorker &fs_worker,
                                        DataBase &db_worker) {
    auto &request_headers = request.get_headers();
    auto &work_dir = request_headers["work_directory"];
    auto &curr_dir = request_headers["current_directory"];
    auto &command = request_headers["command"];

    write_to_logs("AUTH_HANDLER", ERROR);

    if (command == "register") {
        register_user(request_headers["email"], request_headers["password"], response, fs_worker, db_worker);
        return;
    }

    if (command == "login") {
        login_user(request_headers["email"], request_headers["password"], response, db_worker);
        return;
    }

    int id = get_id_from_jwt(request_headers["jwt"]);
    if (id == -1) {
        response = create_response(HttpStatusCode::Forbidden);
        return;
    }

    write_to_logs("---------------", ERROR);
    write_to_logs(curr_dir, ERROR);
    write_to_logs(work_dir, ERROR);

    curr_dir.erase(curr_dir.begin());

    if (work_dir.ends_with('/')) {
        work_dir.pop_back();
    }

    std::string abs_work_dir = (std::filesystem::path(request_headers["current_directory"]) /
                                std::filesystem::path(request_headers["work_directory"]) ).lexically_normal();

    if (abs_work_dir.ends_with('/')) {
        abs_work_dir.pop_back();
    }

    write_to_logs(abs_work_dir, ERROR);

    if (command == "ls") {
        get_user_dir_list_files(id, abs_work_dir, response, db_worker);
        return;
    }

    if (command == "cd") {
        change_user_dir(id, abs_work_dir, response, db_worker);
        return;
    }

    if (command == "mkdir") {
        make_user_subdir(id, abs_work_dir, response, fs_worker, db_worker);
        return;
    }

    if (command == "rmdir") {
        remove_user_subdir(id, abs_work_dir, response, fs_worker, db_worker);
        return;
    }

        if (request_headers["command"] == "download") {
            download_file_from_server(id, abs_work_dir ,request_headers["filename"], response, fs_worker );
            return;
        }

    if (request_headers["command"] == "upload") {
        upload_file_to_server(id, abs_work_dir, request_headers["filename"], request.get_body(), response, fs_worker,
                              db_worker);
        return;
    }

    response = create_response(HttpStatusCode::BadRequest, {});
    return;
}

void RequestHandlerAuth::get_user_dir_list_files(int id, const std::string &work_dir, HttpResponse &response,
                                                 DataBase &db_worker) {
    try {
        write_to_logs(work_dir, ERROR);
        std::string body = db_worker.single_auth_mode.get_list_files_in_dir(id, work_dir);
        if (!body.empty()) {
            write_to_logs(std::to_string(__LINE__), ERROR);
            response = create_response(HttpStatusCode::OK, {}, std::move(body));
            return;
        }
    }
    catch (const std::string &e) {
        write_to_logs(std::to_string(__LINE__), ERROR);
        write_to_logs(e, ERROR);
    }
    write_to_logs(std::to_string(__LINE__), ERROR);
    response = create_response(HttpStatusCode::InternalServerError, {{"command", "ls"}});
}

void RequestHandlerAuth::change_user_dir(int id, const std::string &work_dir, HttpResponse &response,
                                         DataBase &db_worker) {
    try {
        if (!db_worker.single_auth_mode.is_dir_name_free(id, work_dir)) {
            response = create_response(HttpStatusCode::OK);
        } else {
            response = create_response(HttpStatusCode::Forbidden);
        }
    }
    catch (const std::string &e) {
        write_to_logs(e, ERROR);
        response = create_response(HttpStatusCode::InternalServerError, {{"command", "cd"}});
    }
}


void RequestHandlerAuth::make_user_subdir(int id, const std::string &work_dir,
                                          HttpResponse &response, FsWorker &fs_worker, DataBase &db_worker) {
//    write_to_logs(std::to_string(__LINE__), ERROR);
//    try {
//        //TODO: удалить проверку, когда Андрей сделает  BOOL create_directory()
//        if (!db_worker.single_auth_mode.is_dir_name_free(id, work_dir)) {
//            write_to_logs(std::to_string(__LINE__), ERROR);
//            response = create_response(HttpStatusCode::Conflict);
//        }
//        write_to_logs(std::to_string(__LINE__), ERROR);
//        db_worker.single_auth_mode.create_directory(id, work_dir);
//    }
//    catch (const std::string &e) {
//        write_to_logs(std::to_string(__LINE__), ERROR);
//        write_to_logs(e, ERROR);
//        response = create_response(HttpStatusCode::InternalServerError);
//    }
//
//    write_to_logs(fs_worker.auth_usr.get_root(), ERROR);
//    write_to_logs(std::to_string(id), ERROR);
//    write_to_logs(work_dir, ERROR);
//    //TODO: curr_dir+ '/'  + work_dir ИЛИ  curr_dir+ '/'  + work_dir
//    if (!fs_worker.auth_usr.make_subdir(work_dir, std::to_string(id))) {
//        write_to_logs(std::to_string(__LINE__), ERROR);
//        write_to_logs(fs_worker.auth_usr.err_code.message(), ERROR);
//        response = create_response(HttpStatusCode::InternalServerError);
//        return;
//    }

    write_to_logs(fs_worker.auth_usr.err_code.message(), ERROR);
    response = create_response(HttpStatusCode::OK, {{"command", "mkdir"}});
}

void RequestHandlerAuth::remove_user_subdir(int id, const std::string &work_dir,
                                            HttpResponse &response, FsWorker &fs_worker, DataBase &db_worker) {
    write_to_logs(std::to_string(__LINE__), ERROR);
    if (work_dir.empty() || work_dir == "/") {
        response = create_response(HttpStatusCode::Forbidden);
        return;
    }
    try {
        //TODO: удалить из БД
    }
    catch (const std::string &e) {
        write_to_logs(std::to_string(__LINE__), ERROR);
        write_to_logs(e, ERROR);
        response = create_response(HttpStatusCode::InternalServerError);
    }

    write_to_logs(fs_worker.auth_usr.get_root(), ERROR);
    write_to_logs(std::to_string(id), ERROR);
    write_to_logs(work_dir, ERROR);
    if (!fs_worker.auth_usr.remove(work_dir, std::to_string(id))) {
        write_to_logs(std::to_string(__LINE__), ERROR);
        write_to_logs(fs_worker.auth_usr.err_code.message(), ERROR);
        response = create_response(HttpStatusCode::InternalServerError);
        return;
    }

    write_to_logs(fs_worker.auth_usr.err_code.message(), ERROR);
    response = create_response(HttpStatusCode::OK, {{"command", "rmdir"}});
}


void RequestHandlerAuth::register_user(const std::string &email, const std::string &password, HttpResponse &response,
                                       FsWorker &fs_worker, DataBase &db_worker) {
    int id = -1;
    write_to_logs(std::to_string(__LINE__), ERROR);
    try {
        write_to_logs(std::to_string(__LINE__), ERROR);
        id = db_worker.reg_auth.try_register(email, password);
        write_to_logs(std::to_string(__LINE__), ERROR);
    }
    catch (const std::string &e) {
        write_to_logs(std::to_string(__LINE__), ERROR);
        write_to_logs(e, ERROR);
        response = create_response(HttpStatusCode::InternalServerError);
        write_to_logs(std::to_string(__LINE__), ERROR);
        return;
    }

    if (id == -1) {
        write_to_logs(std::to_string(__LINE__), ERROR);
        response = create_response(HttpStatusCode::Conflict);
        return;
    }

    std::string jwt = create_jwt(id);

    if (jwt.empty()) {
        write_to_logs(std::to_string(__LINE__), ERROR);
        response = create_response(HttpStatusCode::InternalServerError);
    }

    if (!fs_worker.auth_usr.add(std::to_string(id))) {
        try {
            write_to_logs(fs_worker.auth_usr.err_code.message(), ERROR);
            write_to_logs(std::to_string(__LINE__), ERROR);
            //TODO: удалить запись из БД
        }
        catch (const std::string &e) {
            write_to_logs(std::to_string(__LINE__), ERROR);
            write_to_logs(e, ERROR);
        }
        response = create_response(HttpStatusCode::InternalServerError);
        return;
    }

    write_to_logs(std::to_string(__LINE__), ERROR);
    response = create_response(HttpStatusCode::OK, {{"jwt", jwt}, {"command", "register"}});
}

void RequestHandlerAuth::login_user(const std::string &email, const std::string &password, HttpResponse &response,
                                    DataBase &db_worker) {
    int id = -1;
    try {
        id = db_worker.reg_auth.try_auth(email, password);
    }
    catch (const std::string &e) {
        write_to_logs(e, ERROR);
        response = create_response(HttpStatusCode::InternalServerError);
        return;
    }
    if (id == -1) {
        response = create_response(HttpStatusCode::Forbidden);
        return;
    }

    std::string jwt = create_jwt(id);
    if (jwt.empty()) {
        response = create_response(HttpStatusCode::InternalServerError);
    }

    response = create_response(HttpStatusCode::OK, {{"jwt", jwt}, {"command", "login"}});
}

std::string RequestHandlerAuth::create_jwt(int id) {
    using namespace jwt::params;
    std::error_code ec;
    return jwt::jwt_object{algorithm(jwt_algorithm), payload({{"id", std::to_string(id)}}), secret(jwt_key)}.signature(
            ec);
}

int RequestHandlerAuth::get_id_from_jwt(const std::string &jwt) {
    using namespace jwt::params;
    std::error_code ec;
    auto jwt_decoded_obj = jwt::decode(jwt, algorithms({jwt_algorithm}), ec, secret(jwt_key));
    return ec ? -1 : std::stoi(jwt_decoded_obj.payload().get_claim_value<std::string>("id"));
}

bool RequestHandlerAuth::download_file_from_server(int id, const std::string &work_dir, const std::string &filename,
                                                   HttpResponse &response, FsWorker &fs_worker) {
    write_to_logs(std::to_string(__LINE__), ERROR);
    write_to_logs(static_cast<std::filesystem::path>(work_dir) / filename, ERROR);

    std::string file_content{file_to_string(
            fs_worker.auth_usr.get_file(static_cast<std::filesystem::path>(work_dir) / filename, std::to_string(id)))};

    if (file_content.empty()) {
        write_to_logs(std::to_string(__LINE__), ERROR);
        response = create_response(HttpStatusCode::InternalServerError, {{"command", "download"}});
        return false;
    }
    write_to_logs(std::to_string(__LINE__), ERROR);
    response = create_response(HttpStatusCode::OK, {{"filename", filename},
                                                    {"command",  "download"}}, std::move(file_content));
    return true;
}


void RequestHandlerAuth::upload_file_to_server(int id, const std::string &work_dir, const std::string &filename,
                                               const std::string &file, HttpResponse &response,
                                               FsWorker &fs_worker, DataBase &db_worker) {
    try {
        write_to_logs(std::to_string(__LINE__), ERROR);
        write_to_logs(std::to_string(id), ERROR);
        write_to_logs(work_dir, ERROR);
        write_to_logs(filename, ERROR);
        if (db_worker.single_auth_mode.add_file(id, work_dir, filename)) {
            response = create_response(HttpStatusCode::Conflict);
            return;
        }
    }
    catch (const std::string &error_msg) {
        write_to_logs(error_msg, ERROR);
        write_to_logs(std::to_string(__LINE__), ERROR);
        response = create_response(HttpStatusCode::InternalServerError, {{"command", "upload"}});
        return;
    }

    catch (const std::string error_msg) {
        write_to_logs(error_msg, ERROR);
        write_to_logs(std::to_string(__LINE__), ERROR);
        response = create_response(HttpStatusCode::InternalServerError, {{"command", "upload"}});
        return;
    }

    catch (const char *error_msg) {
        write_to_logs(std::string(error_msg), ERROR);
        write_to_logs(std::to_string(__LINE__), ERROR);
        response = create_response(HttpStatusCode::InternalServerError, {{"command", "upload"}});
        return;
    }

    if (!fs_worker.auth_usr.write_to_file(file, static_cast<std::filesystem::path>(work_dir) / filename, std::to_string(id))) {
        db_worker.single_auth_mode.delete_file(id, work_dir, filename);
        write_to_logs(fs_worker.auth_usr.err_code.message(), ERROR);
        response = create_response(HttpStatusCode::InternalServerError, {{"command", "upload"}});
        return;
    }

    response = create_response(HttpStatusCode::OK, {{"command", "upload"}}, {});
    return;
}
