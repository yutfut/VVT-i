#include <http_response.h>
#include <http_status_code.h>
#include <log.h>
#include <irequest_handler.h>
#include <http_headers.h>


IRequestHandler::IRequestHandler(std::vector<Log *> &vector_logs): vector_logs(vector_logs){}

HttpResponse
IRequestHandler::create_response(HttpStatusCode status, std::map<std::string, std::string> &&additional_headers,
                                 std::string &&body) {
    additional_headers.merge(
            std::map<std::string, std::string>{{http_headers::content_length, std::to_string(body.size())},
                                               {http_headers::status,         std::to_string(status)},
                                               {http_headers::message,        get_message(status)}});
    return {std::move(additional_headers), std::move(body), 1, 1, status, get_message(status)};
}


void IRequestHandler::write_to_logs(const std::string &message, bl::trivial::severity_level lvl) {
    for (auto &vector_log: this->vector_logs) {
        vector_log->log(message, lvl);
    }
}
