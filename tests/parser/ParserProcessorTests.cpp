#include "gtest/gtest.h"

#include "ParserProcessor.h"
#include "../../src/parser/ParserProcessor.h"

static const char *test_config = "tests/parser/test_config";

/** 
 * Test a simple situation 
 */
TEST(ParserProcessorTest, GetPort1)
{   
    // Parse a mocked input
    const std::string config_text = "port 8080;";
    std::stringstream config_stream(config_text);
    NginxConfigParser parser;
    NginxConfig config;
    parser.Parse(&config_stream, &config);

    // Make sure that the port number is as expected
    ParserProcessor parser_processor = ParserProcessor(config);
    unsigned short port = parser_processor.get_port();
    EXPECT_EQ(8080, port);
}

/** 
 * Test a situation where the port is in a different line in a file 
 */
TEST(ParserProcessorTest, GetPort2)
{   
    // Parse a mocked input
    const std::string config_text = "port \n \n 8080;";
    std::stringstream config_stream(config_text);
    NginxConfigParser parser;
    NginxConfig config;
    parser.Parse(&config_stream, &config);
    
    // Make sure that the port number is as expected
    ParserProcessor parser_processor = ParserProcessor(config);
    unsigned short port = parser_processor.get_port();
    EXPECT_EQ(8080, port);
}

/** 
 * Test a situation where the port is within a block in a file 
 * with other items in it. 
 */
TEST(ParserProcessorTest, GetPortFile)
{	
    // Parse the input test file
	NginxConfigParser config_parser;
    NginxConfig config;
    config_parser.Parse(test_config, &config);
    ParserProcessor parser_processor = ParserProcessor(config);
    
    // Make sure that the port number is as expected
    unsigned short port = parser_processor.get_port();
    EXPECT_EQ(8080, port);
}

/**
 * Test a simple situation
 */
TEST(ParserProcessorTest, GetPath1)
{
    // Parse a mocked input
    const std::string config_text("location1 /static/files;");
    std::stringstream config_stream(config_text);
    NginxConfigParser parser;
    NginxConfig config;
    parser.Parse(&config_stream, &config);

    // Make sure that the paths are as expected
    ParserProcessor parser_processor = ParserProcessor(config);
    strmap *paths = parser_processor.get_paths();

    strmap expected_values({"location1", "/static/files"});
    EXPECT_EQ(*paths, expected_values);
}
