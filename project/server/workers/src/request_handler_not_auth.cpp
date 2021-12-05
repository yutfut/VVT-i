#include <request_handler_not_auth.h>
#include <http_status_code.h>

bool
RequestHandlerNotAuth::handle_request(HttpRequest &request, HttpResponse &response) {
    std::string command =request.get_headers()["command"];
    std::string action (command, 0 , command.find(' '));
    if ( action == "dld_file") {
        return RequestHandlerNotAuth::download_file(request.get_headers()["filename"], request.get_body(), response);
    }
    if (action == "upld_file") {
        return RequestHandlerNotAuth::upload_file(request.get_headers()["file_id"], request.get_headers()["opt_pswd"],
                                                  response);
    }
    response = HttpResponse({}, {}, request.get_major(), request.get_minor(), HttpStatusCode::BadRequest,
                            get_message(HttpStatusCode::BadRequest));
    return false;
}

bool RequestHandlerNotAuth::download_file(const std::string &filename, const std::string &file,
                                          HttpResponse &response) {

    //обращение к бд
    // обработка ошибок бд
    // обращение к ФС
    // обработка ошибок ФС + удаление из БД если надо
    response = HttpResponse({}, {}, request.get_major(), request.get_minor(), HttpStatusCode::BadRequest,
                            get_message(HttpStatusCode::BadRequest));
    return false;
}

