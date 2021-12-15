#pragma once

#include "http_request.h"
#include "http_response.h"

enum HttpStatusCode {
    OK=200,
    BadRequest = 400,
    Forbidden = 403,
    RequestTimeout = 408,
    InternalServerError = 500
};

std::string get_message(HttpStatusCode status);