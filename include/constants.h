#pragma once

#include <string>

///----CREATE TABLES---///


const std::string CREATE_EXTENSION_UUID = "CREATE EXTENSION IF NOT EXISTS \"uuid-ossp\";";

const std::string CREATE_EXTENSION_PGCRYPTO = "CREATE EXTENSION IF NOT EXISTS \"pgcrypto\";";

const std::string CREATE_TABLE_UNAUTH_USER_FILES = "CREATE TABLE IF NOT EXISTS Unauth_user_files("
                                                   "uuid UUID PRIMARY KEY DEFAULT uuid_generate_v4(), user_filename TEXT, password TEXT, "
                                                   "upload_date DATE DEFAULT CURRENT_DATE);";

const std::string CREATE_TABLE_USERS = "CREATE TABLE IF NOT EXISTS Users(id SERIAL PRIMARY KEY, username TEXT NOT NULL, "
                                       "email TEXT NOT NULL, password TEXT NOT NULL);";

const std::string CREATE_TABLE_PERSON_DIR = "CREATE TABLE IF NOT EXISTS Person_dir(user_id SERIAL REFERENCES Users(id), "
                                            "dir_storage_path TEXT);";

const std::string CREATE_TABLE_GROUPS = "CREATE TABLE IF NOT EXISTS Groups(id SERIAL PRIMARY KEY, dir_storage_path TEXT UNIQUE);";

const std::string CREATE_TABLE_GROUP_DIR = "CREATE TABLE IF NOT EXISTS Group_dir("
                                           "user_id SERIAL REFERENCES Users(id), group_id SERIAL REFERENCES Groups(id), group_name TEXT, modifiers TEXT);";

const std::string CREATE_TABLE_AUTH_USER_FILES = "CREATE TABLE IF NOT EXISTS Auth_user_files("
                                                 "user_id SERIAL REFERENCES Users(id), "
                                                 "dir_path TEXT REFERENCES Person_dir(dir_storage_path));";

///---Requests to DB---///

std::string ADD_UNAUTH_USER_FILE = "INSERT INTO Unauth_user_files(user_filename, password) VALUES('{0}', '{1}') RETURNING uuid, upload_date;";
std::string DELETE_FILE_BY_dATE = "DELETE FROM Unauth_user_files WHERE upload_date = '{0}';";
std::string GET_UPLOAD_DATE = "SELECT user_filename, upload_date FROM Unauth_user_files WHERE uuid = '{0}' AND password = '{1}';";
std::string DELETE_CERTAIN_UNAUTH_FILE = "DELETE FROM Unauth_user_files WHERE uuid = '{0}';";
