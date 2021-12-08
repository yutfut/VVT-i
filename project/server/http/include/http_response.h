#pragma once

#include "http_base.h"

class HttpResponse : public HttpBase {
public:
    HttpResponse() = default;

    HttpResponse(const std::map<std::string, std::string> &headers, const std::string &body,

                    int major,
                    int minor,
                    int status,
                    const std::string &message);

    HttpResponse(std::map<std::string, std::string> &&headers, std::string &&body,
                 int major,
                 int minor,
                 int status,
                 std::string &&message);

    HttpResponse &operator=(HttpResponse &&src)  noexcept;

    ~HttpResponse() = default;

    std::string get_string();

    int get_status() const;

private:
    int status = 0;

    std::string message;
};
