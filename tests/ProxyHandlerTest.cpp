#include "gtest/gtest.h"

#include "../../src/server/ProxyHandler.h"
#include <cstdio>

static const char *test_config = "tests/parser/test_config";

/* 
 * Test for ProxyHandler
 */

class RequestHandlerTest : public ::testing::Test {
    // Added 5/7/2015
    // Assignment 5 unit tests

    // Modified 5/24/2015 to work with new handlers.
    protected:

    bool HandleProxyResponse(const HTTPRequest &req, 
            const std::string &host, const std::string &port, const std::string &expected) {
        ProxyHandler a;
        std::string output;
        size_t check_http;
        size_t check_domain;
        size_t check_content;
        size_t check_date;

        NginxConfig config;
        std::shared_ptr<NginxConfigStatement> statement (new NginxConfigStatement);
        statement->tokens_.push_back("host");
        statement->tokens_.push_back(host);
        config.statements_.push_back(port);
        a.Configure(config);

        output = a.HandleRequest(req);


        check_http = output.find("HTTP/1.0 200 OK\r\n");
        check_domain = output.find(expected);
        check_content_type = output.find("Content-Type: text/html;\r\n");     
        check_date = output.find("GMT");

        if (check_http == std::string::npos ||
            check_content_type == std::string::npos ||
            check_date == std::string::npos ||
            check_domain == std::string::npos) {
            return 0;
        }
        return 1;     
    }  

};

// Request Handler Tests:
TEST_F(RequestHandlerTest, HandleProxyResponse) {
    // Tests the google.com proxy from port 80.
    HTTPRequest test_static1;
    test_static1.method = "GET";
    test_static1.path = "/reverse"; 
    EXPECT_TRUE(HandleProxyResponse(test_static1, "www.google.com", "80", "domain=.google.com"));


}
