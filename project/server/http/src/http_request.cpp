#include "http_request.h"

#include <iostream>

std::string HttpRequest::get_method() const {
    return this->method;
}

std::string HttpRequest::get_url() const {
    return this->url;
}

void HttpRequest::add_line(const std::string& line) {
    if (!this->first_line_added) {
        add_first_line(line);
        return;
    }
    if (!this->headers_added) {
        add_header(line);
        return;
    }

    add_body(line);
}

void HttpRequest::add_first_line(const std::string& line) {
    size_t lf_pos = line.find('\n');
    if (lf_pos == std::string::npos) {
        throw "Line feed not found";
    }

    size_t start_pos = 0;
    size_t end_pos = line.find(' ');
    if (end_pos == std::string::npos) {
        throw "Space not found";
    }
    this->method = std::string(line, start_pos, end_pos - start_pos);

    start_pos = end_pos + 1;
    while (line[start_pos] == ' ') {
        ++start_pos;
    }
    end_pos = line.find(' ', start_pos);
    if (end_pos == std::string::npos) {
        throw "Space not found";
    }
    this->url = std::string(line, start_pos, end_pos - start_pos);

    start_pos = end_pos + 1;
    while (line[start_pos] == ' ') {
        ++start_pos;
    }

    std::string protocol(line, start_pos, lf_pos - 1 - start_pos);

    if (sscanf(protocol.c_str(), "HTTP/%d.%d", &version_major, &version_minor) != 2) {
        throw "Error while reading from file descriptor";
    }

    this->first_line_added = true;
}

void HttpRequest::add_header(const std::string& line) {
    size_t lf_pos = line.find('\n');
    if (lf_pos == std::string::npos) {
        throw "Line feed not found";
    }

    size_t start_pos = 0;
    if (lf_pos == start_pos || line[start_pos] == '\r') {
        this->headers_added = true;

        if (this->headers.find("content-length") == this->headers.end() || this->headers["content-length"] == "0" ) {
            this->request_ended = true;
        }

        return;
    }

    size_t colon_pos = line.find(':');
    if (colon_pos == std::string::npos) {
        throw "Colon not found";
    }

    std::string header_name(line, start_pos, colon_pos - start_pos);
    std::transform(header_name.begin(),
        header_name.end(),
        header_name.begin(),
        [](unsigned char c) { return std::tolower(c); });

    start_pos = colon_pos + 1;
    while (line[start_pos] == ' ') {
        ++start_pos;
    }

    std::string header_value(line, start_pos, lf_pos - 1 - start_pos);
    this->headers[header_name] = header_value;
}

void HttpRequest::add_body(const std::string& line) {
    int len;
    if (sscanf(this->headers["content-length"].c_str(), "%d", &len) != 1) {
        // TODO: catch expection
        // throw "Not valid request";
    }

    if (curr_len == -1) {
        curr_len = len;
    }

    this->body += line;

    curr_len -= line.length();
    if (curr_len <= 0) {
        this->request_ended = true;
        this->body.erase(this->body.rfind("\r\n"), 2);
    }
}

bool HttpRequest::requst_ended() const {
    return this->request_ended;
}
