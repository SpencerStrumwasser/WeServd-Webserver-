#include "gtest/gtest.h"
#include "../../src/webserver.cpp"
#include "../../src/parser/ParserProcessor.h"

TEST(SamePort,GetPort)
{	NginxConfigParser config_parser;
    NginxConfig config;
    config_parser.Parse("../../default_config", &config);
    ParserProcessor parser_processor = ParserProcessor(config);
    unsigned short port = parser_processor.get_port();
    EXPECT_EQ(8080, port);

}
