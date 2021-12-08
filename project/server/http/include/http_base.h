#pragma once

#include <string>
#include <map>
#include <unistd.h>

#include "http_defines.h"

class HttpBase {
public:
    HttpBase() = default;

     HttpBase(const std::map<std::string, std::string>& headers, std::string body = "",
            int major = -1, int minor = -1);

    HttpBase& operator=(HttpBase&& src) noexcept;

    ~HttpBase() = default;

    int get_minor() const;

    int get_major() const;

    std::map<std::string, std::string>& get_headers();

    std::string get_body() const;

protected:
    int version_major;

    int version_minor;

    std::map<std::string, std::string> headers;

    std::string body;
};
