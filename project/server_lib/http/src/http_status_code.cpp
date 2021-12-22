#include "http_status_code.h"

std::string get_message(HttpStatusCode status){
    switch (status) {
        case HttpStatusCode::OK:
            return {"OK"};
        case HttpStatusCode::BadRequest:
            return {"BadRequest"};
        case HttpStatusCode::Forbidden:
            return {"Forbidden"};
        case HttpStatusCode::RequestTimeout:
            return {"RequestTimeout"};
        case HttpStatusCode::Conflict:
            return {"Conflict"};
        case HttpStatusCode::InternalServerError:
            return {"InternalServerError"};
        default:
            return {""};
    }
}
