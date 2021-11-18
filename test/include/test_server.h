#include <gtest/gtest.h>
#include <unordered_map>
#include <sys/types.h>
#include <unistd.h>

#include "server.h"


TEST(TEST_CONFIG, HANDLE_EMPTY_CONFIG) {
    auto settings = MainServerSettings("test_settings/default.conf");

    EXPECT_EQ(settings.get_count_workflows(), 4);
    EXPECT_EQ(settings.get_access_log_filename(), "access.log");
    EXPECT_EQ(settings.get_error_log_filename(), "error.log");

    EXPECT_EQ(settings.get_server().get_port(), 80);
    EXPECT_EQ(settings.get_server().get_servername(), "127.0.0.1");
}

TEST(TEST_CONFIG, HANDLE_INVALID_CONFIG) {
    EXPECT_ANY_THROW({ auto settings = MainServerSettings("test_settings/invalid.conf"); });
}

TEST(TEST_CONFIG, HandleConfig) {
    auto settings = MainServerSettings("test_settings/test.conf");

    EXPECT_EQ(settings.get_count_workflows(), 2);
    EXPECT_EQ(settings.get_access_log_filename(), "test_access.log");
    EXPECT_EQ(settings.get_error_log_filename(), "test_error.log");

    EXPECT_EQ(settings.get_server().get_port(), 3000);
    EXPECT_EQ(settings.get_server().get_servername(), "192.168.0.1");
}

TEST(TEST_HTTP, HANDLE_HTTP_REQUEST_WITH_BODY) {
    std::string str = ""
        "GET /index.html HTTP/1.1\n"
        "Host: VVT-i\n"
        "Content-Length: 23\n"
        "Content-Type: text/html\n"
        "\n"
        "<div>\n"
        "\tHELLO WORLD!\n"
        "</div>\n";

    size_t pos;
    HttpRequest request;

    while ((pos = str.find("\n")) != std::string::npos) {
        auto line = str.substr(0, pos + 1);
        request.add_line(line);
        str.erase(0, pos + 1);
    }

    EXPECT_EQ(request.get_method(), "GET");
    EXPECT_EQ(request.get_url(), "/index.html");

    EXPECT_EQ(request.get_headers()["host"], "VVT-i");
    EXPECT_EQ(request.get_headers()["content-length"], "23");
    EXPECT_EQ(request.get_headers()["content-type"], "text/html");

    EXPECT_EQ(request.get_body(), "<div>\n\tHELLO WORLD!\n</div>\n");
}

TEST(TEST_HTTP, HANDLE_HTTP_REQUEST_WITHOUT_BODY) {
    std::string str = ""
        "GET /index.html HTTP/1.1\n"
        "Host: VVT-i\n"
        "\n";

    size_t pos;
    HttpRequest request;

    while ((pos = str.find("\n")) != std::string::npos) {
        auto line = str.substr(0, pos + 1);
        request.add_line(line);
        str.erase(0, pos + 1);
    }

    EXPECT_EQ(request.get_method(), "GET");
    EXPECT_EQ(request.get_url(), "/index.html");

    EXPECT_EQ(request.get_headers()["host"], "VVT-i");
}

TEST(TEST_HTTP, HANDLE_HTTP_RESPONSE_WITH_BODY) {
    std::string result = ""
        "HTTP/1.1 200 OK\n"
        "Server: VVT-i\n"
        "Content-Length: 26\n"
        "Content-Type: text/html\n"
        "\n"
        "<div>\n"
        "\tHELLO WORLD!\n"
        "</div>\n";

    std::unordered_map<std::string, std::string> headers;
    headers[CONTENT_TYPE_HDR] = "text/html";
    headers[CONTENT_LENGTH_HDR] = "26";
    headers[SERVER_HDR] = SERVER_VL;

    HttpResponse response(headers, "<div>\n\tHELLO WORLD!\n</div>\n", 1, 1, 200, "OK");

    EXPECT_EQ(response.get_string(), result);
}

TEST(TEST_HTTP, HANDLE_HTTP_RESPONSE_WITHOUT_BODY) {
    std::string result = ""
        "HTTP/1.1 200 OK\n"
        "Server: VVT-i\n"
        "\n";

    std::unordered_map<std::string, std::string> headers;
    headers[SERVER_HDR] = SERVER_VL;

    HttpResponse response(headers, "", 1, 1, 200, "OK");

    EXPECT_EQ(response.get_string(), result);
}

TEST(TEST_HTTP, HANDLE_INVALID_HTTP_REQUEST) {
    std::string str = "something is here\n";

    HttpRequest request;

    EXPECT_ANY_THROW({ request.add_line(str); });
}

// TEST(TEST_SERVER, HANDLE_SERVER_HARD_COMMANDS) {
//     remove("pid_file.txt");
//     remove("test_access.log");
//     remove("test_error.log");

//     pid_t pid = fork();
//     EXPECT_NE(pid, -1);

//     // For child
//     if (pid == 0) {
//         Server server("test_settings/hard.conf");
//         server.start();

//         exit(0);
//     }

//     // Wait for child to start
//     while(access("pid_file.txt", 0)) {
//         sleep(1);
//     }

//     EXPECT_EQ(access("pid_file.txt", 0), 0);
//     EXPECT_EQ(access("test_access.log", 0), 0);

//     std::string line;
//     std::ifstream file("pid_file.txt");

//     EXPECT_TRUE(file.is_open());
//     EXPECT_FALSE(file.peek() == EOF);

//     std::getline(file, line);
//     pid = std::stoi(line);

//     file.close();

//     remove("pid_file.txt");

//     kill(pid, SIGALRM);
//     // Wait for child to restart
//     while(access("pid_file.txt", 0)) {
//         sleep(1);
//     }

//     file.open("pid_file.txt");

//     EXPECT_TRUE(file.is_open());
//     EXPECT_FALSE(file.peek() == EOF);

//     std::getline(file, line);
//     pid_t new_pid = std::stoi(line);

//     EXPECT_NE(pid, new_pid);

//     file.close();

//     // Send stop hard signal to the new child proccess
//     int status;
//     kill(new_pid, SIGINT);
//     waitpid(new_pid, &status, 0);

//     EXPECT_EQ(WIFEXITED(status), 0);

//     remove("test_access.log");
//     remove("test_error.log");
// }

// TEST(TEST_SERVER, HANDLE_SERVER_SOFT_COMMANDS) {
//     remove("pid_file.txt");
//     remove("test_access.log");
//     remove("test_error.log");

//     pid_t pid = fork();
//     EXPECT_NE(pid, -1);

//     // For child
//     if (pid == 0) {
//         Server server("test_settings/soft.conf");
//         server.start();

//         exit(0);
//     }

//     // Wait for child to start
//     while(access("pid_file.txt", 0)) {
//         sleep(1);
//     }

//     EXPECT_EQ(access("pid_file.txt", 0), 0);
//     EXPECT_EQ(access("test_access.log", 0), 0);

//     std::string line;
//     std::ifstream file("pid_file.txt");

//     EXPECT_TRUE(file.is_open());
//     EXPECT_FALSE(file.peek() == EOF);

//     std::getline(file, line);
//     pid = std::stoi(line);

//     file.close();

//     remove("pid_file.txt");

//     kill(pid, SIGPIPE);
//     // Wait for child to restart
//     while(access("pid_file.txt", 0)) {
//         sleep(1);
//     }

//     file.open("pid_file.txt");

//     EXPECT_TRUE(file.is_open());
//     EXPECT_FALSE(file.peek() == EOF);

//     std::getline(file, line);
//     pid_t new_pid = std::stoi(line);

//     EXPECT_NE(pid, new_pid);

//     file.close();

//     // Send stop hard signal to the new child proccess
//     int status;
//     kill(new_pid, SIGHUP);
//     waitpid(new_pid, &status, 0);

//     EXPECT_EQ(WIFEXITED(status), 0);

//     remove("test_access.log");
//     remove("test_error.log");
// }
