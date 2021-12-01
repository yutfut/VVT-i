#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <string>
#include <utility>
#include <fstream>

#include "server_settings.h"

class MainServerSettings {
public:
    MainServerSettings() = default;

    explicit MainServerSettings(const std::string& config_filename);

    ~MainServerSettings() = default;

    static const std::vector<std::string> valid_properties;

    int get_number_of_property(const std::string& property);

    void set_property(int number_of_property, const std::string& value);

    friend void parse_config(MainServerSettings& server);

    int get_count_workflows();

    ServerSettings get_server();

    std::string get_access_log_filename();

    std::string get_error_log_filename();

private:
    typedef enum {
        S_START,
        S_BRACE_OPEN,
        S_KEY,
        S_VALUE,
        S_SERVER_START,
        S_DATABASE_START,
        S_COUNT,
        S_ERR,
        S_END
    } state_t;

    typedef enum {
        L_PROTOCOL,
        L_BRACE_OPEN,
        L_BRACE_CLOSE,
        L_NEW_LINE,
        L_KEY,
        L_VALUE,
        L_SERVER_START,
        L_DATABASE_START,
        L_DATABASE_END,
        L_SERVER_END,
        L_COUNT,
        L_ERR
    } lexeme_t;

    std::string get_string_from_file(const std::string& filename);

    void parse_config();

    int get_lexeme(const std::string& config, int& pos, const std::vector<std::string>& valid_properties);

    std::string config_filename;

    int count_workflows = 4;

    std::string access_log_filename = "access.log", error_log_filename = "error.log";

    ServerSettings server;

    typedef enum {
        COUNT_WORKFLOWS_NUMBER = 1,
        ACCESS_LOG_NUMBER = 2,
        ERROR_LOG_NUMBER = 3
    } numbers_of_properties;
};
