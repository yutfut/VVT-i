#include <string>

namespace http_headers {
    const std::string command{"command"};
    const std::string filename{"filename"};
    const std::string password{"password"};
    const std::string email{"email"};
    const std::string status{"status"};
    const std::string message{"message"};
    const std::string content_length{"content-length"};
    const std::string key{"key"};
    const std::string jwt{"jwt"};
    const std::string work_directory{"work_directory"};
    const std::string current_directory{"current_directory"};
}

namespace http_commands {
    const std::string upload{"upload"};
    const std::string download{"download"};
    const std::string mkdir{"mkdir"};
    const std::string rmdir{"rmdir"};
    const std::string signup{"register"};
    const std::string login{"login"};
    const std::string get_dirs_content{"ls"};
    const std::string change_directory{"cd"};
    const std::string has{"has"};
    const std::string remove{"rm"};
}

