#pragma once

#include "library_list.h"

static std::string get_current_date() {
    auto convert_number_to_string = [](int num) {
        if (num < 10) {
            return "0" + std::to_string(num);
        }
        return std::to_string(num);
    };

    std::time_t t = std::time(0);
    std::tm *now = std::localtime(&t);
    std::string curr_date;
    curr_date += std::to_string(now->tm_year + 1900) + "-" + convert_number_to_string(now->tm_mon + 1)
                 + "-" + convert_number_to_string(now->tm_mday);

    return curr_date;
}
