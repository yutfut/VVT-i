#include <request_handler_not_auth.h>
#include <http_status_code.h>

bool
RequestHandlerNotAuth::handle_request(HttpRequest &request, HttpResponse &response) {
    if ( request.get_headers()["command"] == "download") {
        return RequestHandlerNotAuth::download_file(request.get_headers()["filename"], request.get_body(), response);
    }
    if ( request.get_headers()["command"] == "upload") {
        return RequestHandlerNotAuth::upload_file(request.get_headers()["filename"], request.get_headers()["password"],
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

