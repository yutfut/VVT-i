#include <gtest/gtest.h>

#include "http_response.h"

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
