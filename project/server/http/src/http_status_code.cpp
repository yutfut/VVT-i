#include "http_status_code.h"


//TODO: изменеить сообщения на обнословные
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

        case HttpStatusCode::InternalServerError:
            return {"InternalServerError"};
        default:
            return {""};
    }
}
