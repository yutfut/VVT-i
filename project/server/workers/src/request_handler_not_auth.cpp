#include <request_handler_not_auth.h>
#include <http_status_code.h>
#include <data_base.h>
#include <fs_worker.h>
#include <fstream>


bool RequestHandlerNotAuth::handle_request(HttpRequest &request, HttpResponse &response, const FsWorker &fs_worker,
                                           const DataBase &db_worker) {
    if (request.get_headers()["command"] == "download") {
        return RequestHandlerNotAuth::download_file(request.get_headers()["filename"], request.get_body(), response,
                                                    fs_worker, db_worker);
    }
    if (request.get_headers()["command"] == "upload") {
        return RequestHandlerNotAuth::upload_file(request.get_headers()["filename"], request.get_headers()["password"],
                                                  response, fs_worker, db_worker);
    }
    response = HttpResponse({}, {}, request.get_major(), request.get_minor(), HttpStatusCode::BadRequest,
                            get_message(HttpStatusCode::BadRequest));
    return false;
}

bool
RequestHandlerNotAuth::download_file(const std::string &file_id, const std::string &opt_pswd, HttpResponse &response,
                                     const FsWorker &fs_worker, const DataBase &db_worker) {
    try {
        if (!db_worker.not_auth_mode.has_access_on_unauth_user_file(file_id, opt_pswd)) {
            response = HttpResponse({}, {}, 0, 0, HttpStatusCode::Forbidden,
                                    get_message(HttpStatusCode::Forbidden)); // TODO: удалить мажор минор
            return false;
        }
    } catch (const std::string &error_msg) {
        //TODO: вывод в лог error_msg
        response = HttpResponse({}, {}, 0, 0, HttpStatusCode::InternalServerError,
                                get_message(HttpStatusCode::InternalServerError)); // TODO: удалить мажор минор
                                //TODO: а в каких случаях кидаетсся исключение? какой ответ правильно отдавать?
        return false;
    }

    std::ifstream file(fs_worker.not_auth_usr.get_file(fs::path("f1"), fs::path("0-0-0"))); // TODO: hardcode
    if (!file.is_open() || fs_worker.not_auth_usr.err_code) {
        response = HttpResponse({}, {}, 0, 0, HttpStatusCode::BadRequest,
                                get_message(HttpStatusCode::BadRequest)); // TODO: удалить мажор минор
        return false;
    }

    response = HttpResponse({}, "123456789", 0, 0, HttpStatusCode::BadRequest,
                            get_message(HttpStatusCode::BadRequest)); // TODO: удалить мажор минор

    // обращение к ФС
    // обработка ошибок ФС + удаление из БД если надо

    return false;
}

