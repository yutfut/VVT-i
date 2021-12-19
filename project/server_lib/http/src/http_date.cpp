#include "http_date.h"

static std::string months[12] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };

static std::string week_days[7] = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };

std::string Date::get_date() {
    std::time_t t = std::time(nullptr);
    struct tm* tm = std::gmtime(&t);
    return get_week_day(tm->tm_wday) + ", " + convert_number_to_string(tm->tm_mday) +
    " " + get_month(tm->tm_mon) + " " + std::to_string(1900 + tm->tm_year) + " " +
    convert_number_to_string(tm->tm_hour) + ":" + convert_number_to_string(tm->tm_min) + ":" +
    convert_number_to_string(tm->tm_sec) + " GMT";
}

std::string Date::get_week_day(int week_day) {
    return week_days[week_day];
}

std::string Date::convert_number_to_string(int num) {
    if (num < 10)
    {
        return "0" + std::to_string(num);
    }
    return std::to_string(num);
}

std::string Date::get_month(int month) {
    return months[month];
}
