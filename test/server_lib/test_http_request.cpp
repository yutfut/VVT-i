#include <gtest/gtest.h>

#include "http_request.h"

TEST(TEST_HTTP, HANDLE_HTTP_REQUEST_WITH_BODY) {
    std::string str = ""
        "GET /index.html HTTP/1.1\r\n"
        "Host: VVT-i\r\n"
        "Content-Length: 28\r\n"
        "Content-Type: text/html\r\n"
        "\r\n"
        "<div>\r\n"
        "\tHELLO WORLD!\r\n"
        "</div>\r\n";

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
    EXPECT_EQ(request.get_headers()["content-length"], "28");
    EXPECT_EQ(request.get_headers()["content-type"], "text/html");

    EXPECT_EQ(request.get_body(), "<div>\r\n\tHELLO WORLD!\r\n</div>");
}

TEST(TEST_HTTP, HANDLE_HTTP_REQUEST_WITHOUT_BODY) {
    std::string str = ""
        "GET /index.html HTTP/1.1\r\n"
        "Host: VVT-i\r\n"
        "\r\n";

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
    EXPECT_EQ(request.get_body(), "");
}

TEST(TEST_HTTP, HANDLE_INVALID_HTTP_REQUEST) {
    std::string str = "something is here\n";

    HttpRequest request;

    EXPECT_ANY_THROW({ request.add_line(str); });
}
