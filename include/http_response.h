//
// Created by Yut Fut on 10.12.2021.
//

#ifndef VVT_I_HTTP_RESPONSE_H
#define VVT_I_HTTP_RESPONSE_H

#include <iostream>
#include <map>
#include <fstream>

#include "user.h"

class HTTPResponse {
public:
    static int parser(std::string &http);
};
#endif //VVT_I_HTTP_RESPONSE_H
