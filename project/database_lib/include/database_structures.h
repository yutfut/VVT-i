#pragma once

#include <string>


typedef struct {
    std::string user;
    std::string password;
    std::string host;
    std::string port;
    std::string dbname;
} database_configuration_t;


typedef struct {
    int additional_bit;
    int user;
    int group;
    int other;
} modifiers_t;


typedef struct {
    std::string uuid;
    std::string filename;
    std::string upload_date;
} unauth_file_data_t;


typedef enum {
    DELETE_GROUP,
    ADD_MEMBER,
    DELETE_MEMBER,
    SET_CHMOD_ON_GROUP,
    SET_CHMOD,
    SET_UMASK,
    UPLOAD,
    DOWNLOAD,
    READ,
    WRITE
} action_t;
