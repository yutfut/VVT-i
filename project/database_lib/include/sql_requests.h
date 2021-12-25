#pragma once

#include <string>
#include <fmt/core.h>


///----CREATE TABLES---///


const std::string CREATE_EXTENSION_UUID = "CREATE EXTENSION IF NOT EXISTS \"uuid-ossp\";";

const std::string CREATE_EXTENSION_PGCRYPTO = "CREATE EXTENSION IF NOT EXISTS \"pgcrypto\";";

const std::string CREATE_TABLE_UNAUTH_USER_FILES = "CREATE TABLE IF NOT EXISTS Unauth_user_files("
                                                   "uuid UUID PRIMARY KEY DEFAULT uuid_generate_v4(), user_filename TEXT, password TEXT, "
                                                   "upload_date DATE DEFAULT CURRENT_DATE);";

const std::string CREATE_TABLE_USERS = "CREATE TABLE IF NOT EXISTS Users(id SERIAL PRIMARY KEY, "
                                       "email TEXT NOT NULL, password TEXT NOT NULL);";

const std::string CREATE_TABLE_PERSON_DIR = "CREATE TABLE IF NOT EXISTS Person_dir(user_id SERIAL REFERENCES Users(id) ON DELETE CASCADE, "
                                            "dir_path TEXT, upload_date DATE DEFAULT CURRENT_DATE);";

const std::string CREATE_TABLE_AUTH_USER_FILES = "CREATE TABLE IF NOT EXISTS Auth_user_files("
                                                 "user_id SERIAL REFERENCES Users(id) ON DELETE CASCADE, upload_date DATE DEFAULT CURRENT_DATE, "
                                                 "dir_path TEXT, user_filename TEXT);";

///---Unauth_mode---///


const std::string ADD_UNAUTH_USER_FILE = "INSERT INTO Unauth_user_files(user_filename, password) VALUES('{0}', crypt('{1}', gen_salt('bf'))) RETURNING uuid, upload_date;";

const std::string DELETE_UNAUTH_USER_FILE = "DELETE FROM Unauth_user_files WHERE uuid = '{0}';";

const std::string DELETE_FILES_BY_DATE = "DELETE FROM Unauth_user_files WHERE upload_date = '{0}';";

const std::string GET_UPLOAD_DATE = "SELECT user_filename, upload_date FROM Unauth_user_files WHERE uuid = '{0}' AND password = crypt('{1}', password);";


///---Reg_auth---///


const std::string REGISTER = "INSERT INTO Users (email, password) VALUES ('{0}', crypt('{1}', gen_salt('bf'))) RETURNING id;";

const std::string CHECK_EMAIL_FREE = "SELECT email FROM Users WHERE email = '{0}';";

const std::string TRY_AUTH = "SELECT id FROM Users WHERE email = '{0}' AND password = crypt('{1}', password);";

const std::string DELETE_USER = "DELETE FROM Users WHERE id = {0};";

const std::string GET_ID_AUTH_USER = "SELECT id FROM Users WHERE email = '{0}';";

const std::string GET_EMAIL_AUTH_USER = "SELECT email FROM Users WHERE id = {0};";


///---Auth mode ---///

const std::string ADD_AUTH_USER_FILE = "INSERT INTO Auth_user_files(user_id, dir_path, user_filename) VALUES({0}, '{1}', '{2}');";

const std::string DELETE_AUTH_USER_FILE = "DELETE FROM Auth_user_files WHERE user_id = {0} AND dir_path = '{1}' AND user_filename = '{2}';";

const std::string DELETE_ALL_FILES_IN_DIR = "DELETE FROM Auth_user_files WHERE user_id = {0} AND dir_path LIKE '{1}%';";

const std::string DELETE_ALL_DIRS_IN_DIR = "DELETE FROM Person_dir WHERE user_id = {0} AND dir_path LIKE '{1}%';";

const std::string CHECK_DIR_NAME_FREE = "SELECT dir_path FROM Person_dir WHERE user_id = {0} AND dir_path = '{1}';";

const std::string CHECK_FILENAME_FREE = "SELECT user_filename FROM Auth_user_files WHERE user_id = {0} AND dir_path = '{1}' AND user_filename = '{2}';";

const std::string CHANGE_FILENAME = "UPDATE Auth_user_files SET user_filename = '{0}' WHERE user_id = {1} AND dir_path = '{2}' AND user_filename = '{3}';";

const std::string COMMAND_MKDIR = "INSERT INTO Person_dir(user_id, dir_path) VALUES({0}, '{1}');";

const std::string COMMAND_LS_FILES = "SELECT user_filename, upload_date FROM Auth_user_files WHERE user_id = {0} AND dir_path = '{1}' ORDER BY user_filename;";

const std::string COMMAND_LS_DIRS = "SELECT dir_path, upload_date FROM Person_dir WHERE user_id = {0} AND dir_path ~ '{1}' ORDER BY dir_path;";
