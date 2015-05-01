#include "gtest/gtest.h"

#include "../../src/parser/ParserProcessor.h"

//Test a simple situation
TEST(SamePort, GetPort)
{	
	NginxConfigParser config_parser;
    NginxConfig config;
    config_parser.Parse("default_config", &config);
    ParserProcessor parser_processor = ParserProcessor(config);
    unsigned short port = parser_processor.get_port();
    EXPECT_EQ(8080, port);

}

//Tests where the port and the number arent on the sae line
TEST(SamePort, GetPort2)
{	
	NginxConfigParser config_parser;
    NginxConfig config;
    config_parser.Parse("default_test_config", &config);
    ParserProcessor parser_processor = ParserProcessor(config);
    unsigned short port = parser_processor.get_port();
    EXPECT_EQ(8080, port);

}


//Tests where the port is not the only thing in the file
TEST(SamePort, GetPort3)
{	
	NginxConfigParser config_parser;
    NginxConfig config;
    config_parser.Parse("test_config", &config);
    ParserProcessor parser_processor = ParserProcessor(config);
    unsigned short port = parser_processor.get_port();
    EXPECT_EQ(8080, port);

}
