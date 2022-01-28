#pragma once

#include <string>

const std::string BASE_ACCESS_LVL = "rwxrwxrwx";

const std::string USER = "postgres";

const std::string PASSWORD = "postgres";

const std::string HOST = "localhost";

const std::string PORT = "5432";

const std::string DBNAME = "postgres";

const std::string ONE_LVL_PATH_REGEX_IN_ROOT = "[-A-Za-z0-9_]+$";

const std::string ONE_LVL_PATH_REGEX = "[/]{1}[-A-Za-z0-9_]+$";

const std::string ROOT_USER_DIR = "/";

const std::string LS_FILES_PATTERN = "-{0}  {1}  {2}\n";

const std::string LS_DIRS_PATTERN = "d{0}  {1}  {2}\n";

const std::string DEFAULT_ROOT_COUT = ".\n";

const std::string DEFAULT_DIR_COUT = ".\n..\n";

const std::string SUCH_DIR_NOT_EXISTS = "ERROR: such directory not exists. Try again\n";
