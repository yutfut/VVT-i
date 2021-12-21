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

const std::string RequestHandlerAuth::jwt_key = "aswt5e^%6*7EyhSFgaS";
const std::string RequestHandlerAuth::jwt_algorithm = "HS256";

static std::string file_to_string(std::ifstream &&file)
{
    if (!file.is_open())
    {
        return {};
    }
    file.seekg(0, std::ios::end);
    size_t file_size = file.tellg();
    std::string str;
    str.resize(file_size);
    file.seekg(0);
    file.read(&str[0], static_cast<std::streamsize>(file_size));
    return str;
}

RequestHandlerAuth::RequestHandlerAuth(std::vector<Log *> &vector_logs) : vector_logs(vector_logs) {}

HttpResponse
RequestHandlerAuth::create_response(HttpStatusCode status, std::map<std::string, std::string> &&additional_headers,
                                    std::string &&body)
{
    additional_headers.merge(std::map<std::string, std::string>{{"content-length", std::to_string(body.size())},
                                                                {"status", std::to_string(status)},
                                                                {"message", get_message(status)}});
    return {std::move(additional_headers), std::move(body), 1, 1, status, get_message(status)};
}

void RequestHandlerAuth::write_to_logs(std::string message, bl::trivial::severity_level lvl)
{
    for (auto &vector_log : this->vector_logs)
    {
        vector_log->log(message, lvl);
    }
}

void RequestHandlerAuth::handle_request(HttpRequest &request, HttpResponse &response, FsWorker &fs_worker,
                                        DataBase &db_worker)
{
    auto &request_headers = request.get_headers();

    write_to_logs("AUTH_HANDLER", ERROR);

    if (request_headers["command"] == "register")
    {
        register_user(request_headers["email"], request_headers["password"], response, fs_worker, db_worker);
        return;
    }

    if (request_headers["command"] == "login")
    {
        login_user(request_headers["email"], request_headers["password"], response, db_worker);
        return;
    }

    int id = get_id_from_jwt(request_headers["jwt"]);
    if (id == -1)
    {
        response = create_response(HttpStatusCode::Forbidden);
        return;
    }

    request_headers["current_directory"].erase(request_headers["current_directory"].begin());
    auto abs_work_dir = (fs::path(request_headers["current_directory"]) / fs::path(request_headers["work_directory"])).lexically_normal();

    if (request_headers["command"] == "ls")
    { 
        get_user_dir_list_files(id, abs_work_dir, response, db_worker);
        return;
    }

    if (request_headers["command"] == "cd")
    { 
        change_user_dir(id, abs_work_dir, response, db_worker);
        return;
    }

    if (request_headers["command"] == "mkdir") {
        make_user_subdir(id, abs_work_dir, response, fs_worker, db_worker);
        return;
    }

    //    if (request_headers["command"] == "download") {
    //        download_file_from_server(request_headers["key"], request_headers["password"], response, fs_worker, db_worker);
    //        return;
    //    }
    //    if (request_headers["command"] == "upload") {
    //        upload_file_to_server(request_headers["filename"], request_headers["password"], request.get_body(), response, fs_worker,
    //                              db_worker);
    //        return;
    //    }
    if (request_headers["command"] == "mkdir")

        response = create_response(HttpStatusCode::BadRequest, {});
    return;
}

void RequestHandlerAuth::get_user_dir_list_files(int id, const std::string &work_dir, HttpResponse &response,
                             DataBase &db_worker)
{
    try
    {
        write_to_logs(work_dir, ERROR);
        std::string body = db_worker.single_auth_mode.get_list_files_in_dir(id, work_dir);
        if (!body.empty())
        {
            write_to_logs(std::to_string(__LINE__), ERROR);
            response = create_response(HttpStatusCode::OK, {}, std::move(body));
            return;
        }
    }
    catch (const std::string &e)
    {
        write_to_logs(std::to_string(__LINE__), ERROR);
        write_to_logs(e, ERROR);
    }
    write_to_logs(std::to_string(__LINE__), ERROR);
    response = create_response(HttpStatusCode::InternalServerError);
}

void RequestHandlerAuth::change_user_dir(int id, const std::string &work_dir, HttpResponse &response,
                     DataBase &db_worker)
{
    try
    {
        if (!db_worker.single_auth_mode.is_dir_name_free(id, work_dir))
        {
            response = create_response(HttpStatusCode::OK);
        }
        else
        {
            response = create_response(HttpStatusCode::Forbidden);
        }
    }
    catch (const std::string &e)
    {
        write_to_logs(e, ERROR);
        response = create_response(HttpStatusCode::InternalServerError);
    }
}


void RequestHandlerAuth::make_user_subdir(int id,  const std::string &work_dir,
                                          HttpResponse &response, FsWorker &fs_worker, DataBase &db_worker) {
    write_to_logs(std::to_string(__LINE__), ERROR);
    //TODO: УДАЛИТЬ СЛЕШ??!!
    try
    {
        //TODO: удалить проверку, когда Андрей сделает  BOOL create_directory()
       if (!db_worker.single_auth_mode.is_dir_name_free(id,  work_dir)) {
           write_to_logs(std::to_string(__LINE__), ERROR);
           response = create_response(HttpStatusCode::Conflict);
       }
        write_to_logs(std::to_string(__LINE__), ERROR);
        db_worker.single_auth_mode.create_directory(id,  work_dir, "");
    }
    catch (const std::string &e)
    {
        write_to_logs(std::to_string(__LINE__), ERROR);
        write_to_logs(e, ERROR);
        response = create_response(HttpStatusCode::InternalServerError);
    }

    write_to_logs(fs_worker.auth_usr.get_root(), ERROR);
    write_to_logs(std::to_string(id), ERROR);
    write_to_logs( work_dir, ERROR);
    //TODO: curr_dir+ '/'  + work_dir ИЛИ  curr_dir+ '/'  + work_dir
    if (!fs_worker.auth_usr.make_subdir( work_dir,std::to_string(id))) {
        write_to_logs(std::to_string(__LINE__), ERROR);
        write_to_logs(fs_worker.auth_usr.err_code.message(), ERROR);
        response = create_response(HttpStatusCode::InternalServerError);
    }

    write_to_logs(fs_worker.auth_usr.err_code.message(), ERROR);
    response = create_response(HttpStatusCode::OK);
}

void RequestHandlerAuth::register_user(const std::string &email, const std::string &password, HttpResponse &response,
                                       FsWorker &fs_worker, DataBase &db_worker)
{
    int id = -1;
    write_to_logs(std::to_string(__LINE__), ERROR);
    try
    {
        write_to_logs(std::to_string(__LINE__), ERROR);
        id = db_worker.reg_auth.try_register(email, password);
        write_to_logs(std::to_string(__LINE__), ERROR);
    }
    catch (const std::string &e)
    {
        write_to_logs(std::to_string(__LINE__), ERROR);
        write_to_logs(e, ERROR);
        response = create_response(HttpStatusCode::InternalServerError);
        write_to_logs(std::to_string(__LINE__), ERROR);
        return;
    }

    if (id == -1)
    {
        write_to_logs(std::to_string(__LINE__), ERROR);
        response = create_response(HttpStatusCode::Conflict);
        return;
    }

    std::string jwt = create_jwt(id);

    if (jwt.empty())
    {
        write_to_logs(std::to_string(__LINE__), ERROR);
        response = create_response(HttpStatusCode::InternalServerError);
    }

    if(!fs_worker.auth_usr.add(std::to_string(id))) {
        try
        {
            write_to_logs(fs_worker.auth_usr.err_code.message(), ERROR);
            write_to_logs(std::to_string(__LINE__), ERROR);
            //TODO: удалить запись из БД
        }
        catch (const std::string &e)
        {
            write_to_logs(std::to_string(__LINE__), ERROR);
            write_to_logs(e, ERROR);
        }
        response = create_response(HttpStatusCode::InternalServerError);
        return;
    }

    write_to_logs(std::to_string(__LINE__), ERROR);
    response = create_response(HttpStatusCode::OK, {{"jwt", jwt}});
}

void RequestHandlerAuth::login_user(const std::string &email, const std::string &password, HttpResponse &response, DataBase &db_worker)
{
    int id = -1;
    try
    {
        id = db_worker.reg_auth.try_auth(email, password);
    }
    catch (const std::string &e)
    {
        write_to_logs(e, ERROR);
        response = create_response(HttpStatusCode::InternalServerError);
        return;
    }
    if (id == -1)
    {
        response = create_response(HttpStatusCode::Forbidden);
        return;
    }

    std::string jwt = create_jwt(id);
    if (jwt.empty())
    {
        response = create_response(HttpStatusCode::InternalServerError);
    }

    response = create_response(HttpStatusCode::OK, {{"jwt", jwt}});
}

std::string RequestHandlerAuth::create_jwt(int id)
{
    using namespace jwt::params;
    std::error_code ec;
    return jwt::jwt_object{algorithm(jwt_algorithm), payload({{"id", std::to_string(id)}}), secret(jwt_key)}.signature(
        ec);
}

int RequestHandlerAuth::get_id_from_jwt(const std::string &jwt)
{
    using namespace jwt::params;
    std::error_code ec;
    auto jwt_decoded_obj = jwt::decode(jwt, algorithms({jwt_algorithm}), ec, secret(jwt_key));
    return ec ? -1 : std::stoi(jwt_decoded_obj.payload().get_claim_value<std::string>("id"));
}
//
//bool RequestHandlerAuth::download_file_from_server(const std::string &file_id, const std::string &opt_pswd,
//                                                   HttpResponse &response,
//                                                   FsWorker &fs_worker, DataBase &db_worker) {
//
//    unauth_file_data_t upload_data;
//    try {
//        write_to_logs("__LINE__", ERROR);
//        write_to_logs(std::to_string(__LINE__), ERROR);
//        upload_data = db_worker.not_auth_mode.get_upload_file_date(file_id, opt_pswd);
//        write_to_logs("__LINE__", ERROR);
//        write_to_logs(std::to_string(__LINE__), ERROR);
//
//        if (upload_data.filename.empty()) {
//            response = create_response(HttpStatusCode::Forbidden, {{"command", "download"}});
//            return false;
//        }
//    }
//    catch (const std::string &error_msg) {
//        write_to_logs(error_msg, ERROR);
//        response = HttpResponse({{"content-length", "0"}}, {}, 1, 1, HttpStatusCode::InternalServerError,
//                                get_message(HttpStatusCode::InternalServerError));
//        return false;
//    }
//
//    std::string file{
//            file_to_string(fs_worker.not_auth_usr.get_file(fs::path(file_id), fs::path(upload_data.upload_data)))};
//
//    if (fs_worker.not_auth_usr.err_code) {
//        write_to_logs(fs_worker.not_auth_usr.err_code.message(), ERROR);
//        response = create_response(HttpStatusCode::InternalServerError, {{"command", "download"}});
//        return false;
//    }
//    response = create_response(HttpStatusCode::OK, {{"filename", upload_data.filename},
//                                                    {"command",  "download"}}, std::move(file));
//    return true;
//}
//
//bool RequestHandlerAuth::upload_file_to_server(const std::string &filename, const std::string &opt_pswd,
//                                               const std::string &file, HttpResponse &response,
//                                               FsWorker &fs_worker, DataBase &db_worker) {
//    unauth_file_data_t file_data;
//    try {
//        write_to_logs(std::to_string(__LINE__), ERROR);
//        write_to_logs("I GO TO add_unauth_user_file()", ERROR);
//        file_data = db_worker.not_auth_mode.add_unauth_user_file(filename, opt_pswd);
//        write_to_logs("I LEAVE add_unauth_user_file()", ERROR);
//        write_to_logs(std::to_string(__LINE__), ERROR);
//        if (file_data.uuid.empty()) {
//            response = create_response(HttpStatusCode::Forbidden, {{"command", "upload"}});
//            return false;
//        }
//    }
//    catch (const std::string &error_msg) {
//        write_to_logs(error_msg, ERROR);
//        response = create_response(HttpStatusCode::InternalServerError, {{"command", "upload"}});
//        return false;
//    }
//
//    fs::path tmpfile_path = fs_worker.not_auth_usr.get_root() / "tmp" / file_data.uuid;
//    std::ofstream tmpfile{tmpfile_path, std::ios_base::binary};
//    //    write_to_logs(std::to_string(__LINE__), ERROR);
//    //    write_to_logs(tmpfile_path, ERROR);
//    //    write_to_logs(std::to_string(file.size()), ERROR);
//    //    write_to_logs(std::to_string(__LINE__), ERROR);
//    //    write_to_logs(file, ERROR);
//    tmpfile << file;
//    tmpfile.close();
//
//    if (!fs_worker.not_auth_usr.move_file_to_fs(tmpfile_path, file_data.uuid, file_data.upload_data)) {
//        db_worker.not_auth_mode.delete_certain_file(file_data.uuid);
//        write_to_logs(fs_worker.not_auth_usr.err_code.message(), ERROR);
//        response = create_response(HttpStatusCode::InternalServerError, {{"command", "upload"}});
//        return false;
//    }
//
//    response = create_response(HttpStatusCode::OK, {{"key",     file_data.uuid},
//                                                    {"command", "upload"}}, {});
//    return true;
//}
