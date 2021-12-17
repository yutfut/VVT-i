#pragma once

#include <string>
#include <ctime>

class Date {
public:
    Date() = delete;

    Date(const Date&) = delete;

    ~Date() = delete;

    static std::string get_date();

private:
    static std::string get_week_day(int week_day);

    static std::string convert_number_to_string(int num);

    static std::string get_month(int month);
};
