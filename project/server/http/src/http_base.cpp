#include "http_base.h"

HttpBase::HttpBase(const std::unordered_map<std::string, std::string>& headers, std::string body,
                    int major, int minor)
    : headers(headers), body(body), version_major(major), version_minor(minor) {}

int HttpBase::get_minor() const {
    return this->version_minor;
}

int HttpBase::get_major() const {
    return this->version_major;
}

std::unordered_map<std::string, std::string>& HttpBase::get_headers() {
    return this->headers;
}

std::string HttpBase::get_body() const {
    return this->body;
}
