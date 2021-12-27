//
// Created by Yut Fut on 10.12.2021.
//

#ifndef VVT_I_HTTP_RESPONSE_H
#define VVT_I_HTTP_RESPONSE_H


#include "user.h"

#include <iostream>
#include <map>
#include <fstream>
#include <sstream>
#include <magic.h>
#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/filter/gzip.hpp>

class HTTPResponse {
public:
    static int parser(std::string &http);
};
#endif //VVT_I_HTTP_RESPONSE_H
