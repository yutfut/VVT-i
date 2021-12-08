#include "http_status_code.h"


std::string get_message(HttpStatusCode status){
    switch (status) {
        case HttpStatusCode::BadRequest:
            return {"The server cannot or will not process the request due to an apparent client error"};
        case HttpStatusCode::Forbidden:
            return {"The request contained valid data and was understood by the server, but the server is refusing action."};
        case HttpStatusCode::RequestTimeout:
            return {"The server timed out waiting for the request. "};

        case HttpStatusCode::InternalServerError:
            return {"A generic error message, given when an unexpected condition was encountered and no more specific message is suitable.[. "};
        default:
            return {""};
    }
}
