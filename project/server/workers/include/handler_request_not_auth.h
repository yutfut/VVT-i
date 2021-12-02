#pragma once
#include <http_request.h>
#include <http_response.h>

class RequestHandlerNotAuth{
public:
    bool handle_request( HttpRequest& request, HttpResponse& response);
private:
    bool download_file(const std::string& filename, const std::string & file, HttpResponse& response);
    bool upload_file(const std::string& file_id, const std::string &optional_password, HttpResponse& response);
};
