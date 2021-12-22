#include "http_response.h"

HttpResponse::HttpResponse(std::map<std::string, std::string> &&headers, std::string &&body,
                           int major,
                           int minor,
                           int status,
                           std::string &&message)
        : HttpBase(std::move(headers), std::move(body), major, minor),
          status(status),
          message(std::move(message)) {}

HttpResponse::HttpResponse(const std::map<std::string, std::string> &headers, const std::string &body,
                           int major,
                           int minor,
                           int status,
                           const std::string &message)
        : HttpBase(headers, body, major, minor),
          status(status),
          message(message) {}



std::string HttpResponse::get_string() {
    std::string str = "HTTP/" + std::to_string(this->get_major()) + "." + std::to_string(this->get_minor()) + " " +
                      std::to_string(this->status) + " " + this->message + "\n";

    for (const auto &header: this->headers) {
        str += header.first + ": " + header.second + "\n";
    }

    if (this->body == "") {
        str += "\n";
    } else {
        str += "\n" + this->body;
    }

    return str;
}

int HttpResponse::get_status() const {
    return status;
}
