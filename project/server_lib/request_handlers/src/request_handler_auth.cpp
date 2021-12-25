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
#include <http_headers.h>

const std::string RequestHandlerAuth::jwt_key = "asWt5e^%6*7E";
const std::string RequestHandlerAuth::jwt_algorithm = "HS256";

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

RequestHandlerAuth::RequestHandlerAuth(std::vector<Log *> &vector_logs) : IRequestHandler(vector_logs) {}


void RequestHandlerAuth::handle_request(HttpRequest &request, HttpResponse &response, FsWorker &fs_worker,
                                        DataBase &db_worker) {
    auto &request_headers = request.get_headers();
    auto &work_dir = request_headers[http_headers::work_directory];
    auto &curr_dir = request_headers[http_headers::current_directory];
    auto &command = request_headers[http_headers::command];


    if (command == http_commands::signup) {
        register_user(request_headers[http_headers::email], request_headers[http_headers::password], response,
                      fs_worker, db_worker);
        return;
    }

    if (command == http_commands::login) {
        login_user(request_headers[http_headers::email], request_headers[http_headers::password], response, db_worker);
        return;
    }

    int id = get_id_from_jwt(request_headers[http_headers::jwt]);
    if (id == -1) {
        response = create_response(HttpStatusCode::Forbidden);
        return;
    }

    curr_dir.erase(curr_dir.begin());

    std::string abs_work_dir = (std::filesystem::path("/") /
                                std::filesystem::path(request_headers[http_headers::current_directory]) /
                                std::filesystem::path(
                                        request_headers[http_headers::work_directory])).lexically_normal();

    if (abs_work_dir.ends_with('/') && abs_work_dir.size() != 1) {
        abs_work_dir.pop_back();
    }

    if (abs_work_dir == "." || abs_work_dir.empty()) {
        abs_work_dir = "/";
    }

    if (command == http_commands::get_dirs_content) {
        get_user_dir_list_files(id, abs_work_dir, response, db_worker);
        return;
    }

    if (command == http_commands::change_directory) {
        change_user_dir(id, abs_work_dir, response, db_worker);
        return;
    }

    if (command == http_commands::mkdir) {
        make_user_subdir(id, abs_work_dir, response, fs_worker, db_worker);
        return;
    }

    if (command == http_commands::rmdir) {
        remove_user_subdir(id, abs_work_dir, response, fs_worker, db_worker);
        return;
    }

    if (command == http_commands::download) {
        download_file_from_server(id, abs_work_dir, request_headers[http_headers::filename], response, fs_worker);
        return;
    }

    if (command == http_commands::upload) {
        upload_file_to_server(id, abs_work_dir, request_headers[http_headers::filename], request.get_body(), response,
                              fs_worker, db_worker);
        return;
    }

    response = create_response(HttpStatusCode::BadRequest, {});
}

void RequestHandlerAuth::get_user_dir_list_files(int id, const std::filesystem::path &work_dir, HttpResponse &response,
                                                 DataBase &db_worker) {
    try {
        std::string body = db_worker.single_auth_mode.get_list_files_in_dir(id, work_dir);
        if (!body.empty()) {

            response = create_response(HttpStatusCode::OK, {}, std::move(body));
            return;
        }
    } catch (const std::string &e) {

        write_to_logs(e, ERROR);
    }

    response = create_response(HttpStatusCode::InternalServerError,
                               {{http_headers::command, http_commands::get_dirs_content}});
}

void RequestHandlerAuth::change_user_dir(int id, const std::filesystem::path &work_dir, HttpResponse &response,
                                         DataBase &db_worker) {
    try {
        if (!db_worker.single_auth_mode.is_dir_name_free(id, work_dir)) {
            response = create_response(HttpStatusCode::OK);
        } else {
            response = create_response(HttpStatusCode::Forbidden);
        }
    } catch (const std::string &e) {
        write_to_logs(e, ERROR);
        response = create_response(HttpStatusCode::InternalServerError,
                                   {{http_headers::command, http_commands::change_directory}});
    }
}


void RequestHandlerAuth::make_user_subdir(int id, const std::filesystem::path &work_dir, HttpResponse &response,
                                          FsWorker &fs_worker, DataBase &db_worker) {

    try {
        if (!db_worker.single_auth_mode.create_directory(id, work_dir.parent_path(), work_dir.filename())) {
            response = create_response(HttpStatusCode::Conflict);
        }
    } catch (const std::string &e) {

        write_to_logs(e, ERROR);
        response = create_response(HttpStatusCode::InternalServerError);
    }

    if (!fs_worker.auth_usr.make_subdir(work_dir, std::to_string(id))) {
        write_to_logs(fs_worker.auth_usr.err_code.message(), ERROR);
        response = create_response(HttpStatusCode::InternalServerError);
        return;
    }

    response = create_response(HttpStatusCode::OK, {{http_headers::command, http_commands::mkdir}});
}

void RequestHandlerAuth::remove_user_subdir(int id, const std::filesystem::path &work_dir, HttpResponse &response,
                                            FsWorker &fs_worker, DataBase &db_worker) {

    if (work_dir.empty() || work_dir == "/") {
        response = create_response(HttpStatusCode::Forbidden);
        return;
    }

    try {
        db_worker.single_auth_mode.rmdir(id, work_dir.parent_path(), work_dir.filename());
    } catch (const std::string &e) {

        write_to_logs(e, ERROR);
        response = create_response(HttpStatusCode::InternalServerError);
    }

    if (!fs_worker.auth_usr.remove(work_dir, std::to_string(id))) {

        write_to_logs(fs_worker.auth_usr.err_code.message(), ERROR);
        response = create_response(HttpStatusCode::InternalServerError);
        return;
    }

    write_to_logs(fs_worker.auth_usr.err_code.message(), ERROR);
    response = create_response(HttpStatusCode::OK, {{http_headers::command, http_commands::rmdir}});
}


void RequestHandlerAuth::register_user(const std::string &email, const std::string &password, HttpResponse &response,
                                       FsWorker &fs_worker, DataBase &db_worker) {
    int id = -1;

    try {

        id = db_worker.reg_auth.try_register(email, password);

    } catch (const std::string &e) {

        write_to_logs(e, ERROR);
        response = create_response(HttpStatusCode::InternalServerError);

        return;
    }

    if (id == -1) {

        response = create_response(HttpStatusCode::Conflict);
        return;
    }

    std::string jwt = create_jwt(id);

    if (jwt.empty()) {

        response = create_response(HttpStatusCode::InternalServerError);
    }

    if (!fs_worker.auth_usr.add(std::to_string(id))) {
        write_to_logs(fs_worker.auth_usr.err_code.message(), ERROR);
    }

    response = create_response(HttpStatusCode::OK, {{http_headers::jwt,     jwt},
                                                    {http_headers::command, http_commands::signup}});
}

void RequestHandlerAuth::login_user(const std::string &email, const std::string &password, HttpResponse &response,
                                    DataBase &db_worker) {
    int id = -1;
    try {
        id = db_worker.reg_auth.try_auth(email, password);
    } catch (const std::string &e) {
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

    response = create_response(HttpStatusCode::OK, {{http_headers::jwt,     jwt},
                                                    {http_headers::command, http_commands::login}});
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
    return bool(ec) ? -1 : std::stoi(jwt_decoded_obj.payload().get_claim_value<std::string>("id"));
}

bool RequestHandlerAuth::download_file_from_server(int id, const std::filesystem::path &work_dir,
                                                   const std::filesystem::path &filename, HttpResponse &response,
                                                   FsWorker &fs_worker) {

    std::string file_content{file_to_string(
            fs_worker.auth_usr.get_file(static_cast<std::filesystem::path>(work_dir) / filename, std::to_string(id)))};

    if (file_content.empty()) {

        response = create_response(HttpStatusCode::InternalServerError,
                                   {{http_headers::command, http_commands::download}});
        return false;
    }

    response = create_response(HttpStatusCode::OK, {{http_headers::filename, filename},
                                                    {http_headers::command,  http_commands::download}},
                               std::move(file_content));
    return true;
}


void RequestHandlerAuth::upload_file_to_server(int id, const std::filesystem::path &work_dir,
                                               const std::filesystem::path &filename, const std::string &file_content,
                                               HttpResponse &response, FsWorker &fs_worker, DataBase &db_worker) {
    try {
        if (!db_worker.single_auth_mode.is_filename_free(id, work_dir, filename)) {
            response = create_response(HttpStatusCode::Conflict);
            return;
        }
        db_worker.single_auth_mode.add_file(id, work_dir, filename);
    } catch (const std::string &error_msg) {
        write_to_logs(error_msg, ERROR);
        response = create_response(HttpStatusCode::InternalServerError,
                                   {{http_headers::command, http_commands::upload}});
        return;
    }

    if (!fs_worker.auth_usr.write_to_file(file_content, static_cast<std::filesystem::path>(work_dir) / filename,
                                          std::to_string(id))) {
        db_worker.single_auth_mode.delete_file(id, work_dir, filename);
        write_to_logs(fs_worker.auth_usr.err_code.message(), ERROR);
        response = create_response(HttpStatusCode::InternalServerError,
                                   {{http_headers::command, http_commands::upload}});
        return;
    }

    response = create_response(HttpStatusCode::OK, {{http_headers::command, http_commands::upload}}, {});
}
