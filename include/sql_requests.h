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

///---Unauth_mode---///

const std::string ADD_UNAUTH_USER_FILE = "INSERT INTO Unauth_user_files(user_filename, password) VALUES('{0}', '{1}') RETURNING uuid, upload_date;";

const std::string DELETE_UNAUTH_USER_FILE = "DELETE FROM Unauth_user_files WHERE uuid = '{0}';";

const std::string DELETE_FILES_BY_DATE = "DELETE FROM Unauth_user_files WHERE upload_date = '{0}';";

const std::string GET_UPLOAD_DATE = "SELECT user_filename, upload_date FROM Unauth_user_files WHERE uuid = '{0}' AND password = '{1}';";

///---Reg_autn---///

const std::string REGISTER = "INSERT INTO Users (username, email, password) VALUES ('{0}', '{1}', '{2}');";

const std::string CHECK_EMAIL_FREE = "SELECT email FROM Users WHERE email = '{0}';";

const std::string TRY_AUTH = "SELECT id FROM Users WHERE email = '{0}' AND password = '{1}';";

const std::string GET_ID_AUTH_USER = "SELECT id FROM Users WHERE email = '{0}';";

const std::string GET_EMAIL_AUTH_USER = "SELECT email FROM Users WHERE id = {0};";

///---Auth mode ---///

const std::string ADD_AUTH_USER_FILE = "INSERT INTO Auth_user_files(user_filename, password) VALUES('{0}', '{1}') RETURNING uuid, upload_date;";

const std::string DELETE_AUTH_USER_FILE = "DELETE FROM Auth_user_files WHERE id = '{0}' AND dir_path = '{1}' AND user_filename = '{2}';";

const std::string CHECK_FILENAME_FREE = "SELECT user_filename FROM Auth_user_files WHERE user_filename = '{0}';";

const std::string CHANGE_FILENAME = "UPDATE Auth_user_files SET user_filename = '{0}' WHERE id = '{1}' AND dir_path = '{2}' AND user_filename = '{3}';";

const std::string LS = "SELECT user_filename, upload_data FROM Auth_user_files WHERE id = '{0}' AND dir_path = '{1}' FULL JOIN person_dir ON dir_path = NEW_DIR_PATH;";
