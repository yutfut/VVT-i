#include <gtest/gtest.h>

#include "http_response.h"

TEST(TEST_HTTP, HANDLE_HTTP_RESPONSE_WITH_BODY) {
    std::string result = ""

        "HTTP/1.1 200 OK\r\n"
        "Content-Length: 26\r\n"
        "Content-Type: text/html\r\n"
        "Server: VVT-i\r\n"
        "\r\n"
        "<div>\r\n"
        "\tHELLO WORLD!\r\n"
        "</div>\r\n";

    std::map<std::string, std::string> headers;
    headers[CONTENT_TYPE_HDR] = "text/html";
    headers[CONTENT_LENGTH_HDR] = "26";
    headers[SERVER_HDR] = SERVER_VL;

    HttpResponse response(headers, "<div>\r\n\tHELLO WORLD!\r\n</div>\r\n", 1, 1, 200, "OK");

    EXPECT_EQ(response.get_string(), result);
}

TEST(TEST_HTTP, HANDLE_HTTP_RESPONSE_WITHOUT_BODY) {
    std::string result = ""
        "HTTP/1.1 200 OK\r\n"
        "Server: VVT-i\r\n"
        "\r\n";

    std::map<std::string, std::string> headers;
    headers[SERVER_HDR] = SERVER_VL;

    HttpResponse response(headers, "", 1, 1, 200, "OK");

    EXPECT_EQ(response.get_string(), result);
}
