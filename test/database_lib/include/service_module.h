#ifndef PROJECTNAME_SERVICE_MODULE_H
#define PROJECTNAME_SERVICE_MODULE_H

#include "library_list.h"


static std::string get_current_date() {
    std::time_t t = std::time(0);
    std::tm *now = std::localtime(&t);
    std::string curr_date;
    curr_date += std::to_string(now->tm_year + 1900) + "-" + std::to_string(now->tm_mon + 1)
                 + "-" + std::to_string(now->tm_mday);

    return curr_date;
}


#endif //PROJECTNAME_SERVICE_MODULE_H
