#include <handler_request_not_auth.h>

bool RequestHandlerNotAuth::handle_request( HttpRequest &request, HttpResponse &response) { //TODO: const &request или как?
    if (request.get_headers()["command"] == "dld_file") {
        return RequestHandlerNotAuth::download_file(request.get_headers()["filename"], request.get_body(), response);

    }
    if (request.get_headers()["command"] == "upld_file") {
        return RequestHandlerNotAuth::upload_file(request.get_headers()["file_id"], request.get_headers()["opt_pswd"], response);
    }
    response.
    return false;
}

bool RequestHandlerNotAuth::download_file(const std::string &filename, const std::string &file,
                                          HttpResponse &response) {
    //обращение к бд
    // обработка ошибок бд
    // обращение к ФС
    // обработка ошибок ФС + удаление из БД если надо
    // логирование
}
