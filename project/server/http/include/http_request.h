#pragma once

#include <cstring>
#include <sstream>
#include <algorithm>

#include "http_base.h"

class HttpRequest : public HttpBase {
public:
    HttpRequest() = default;

    ~HttpRequest() = default;

    std::string get_method() const;

    std::string get_url() const;

    void add_line(const std::string& line);

    bool requst_ended() const;

private:
    void add_first_line(const std::string& line);

    void add_header(const std::string& line);

    void add_body(const std::string& line);

private:
    std::string method;

    std::string url;

    bool first_line_added = false;

    bool headers_added = false;

    bool body_added = false;

    int curr_len = -1;

    bool request_ended = false;
};
