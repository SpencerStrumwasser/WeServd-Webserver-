#include "gtest/gtest.h"

#include "ParserProcessor.h"
#include "../../src/parser/ParserProcessor.h"
#include "../../src/server/ProxyHandler.h"
#include <cstdio>

static const char *test_config = "tests/parser/test_config";

/* --- Function Prototypes --- */
void print_map(strmap *string_map);

/** 
 * Test a simple situation 
 */
TEST(ParserProcessorTest, GetPort1)
{   
    // Parse a mocked input
    const std::string config_text = "listen 8080;";
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
    const std::string config_text = "listen \n \n 8080;";
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
    
    // Make sure that the port number is as expected
    ParserProcessor parser_processor = ParserProcessor(config);
    unsigned short port = parser_processor.get_port();
    EXPECT_EQ(8080, port);
}

/**
 * Test a simple situation
 */
TEST(ParserProcessorTest, GetPath1)
{
    // Parse a mocked input
    const std::string config_text("handler static /files {\n root files;\n}");
    std::stringstream config_stream(config_text);
    NginxConfigParser parser;
    NginxConfig config;
    parser.Parse(&config_stream, &config);

    // Make sure that the paths are as expected
    ParserProcessor parser_processor = ParserProcessor(config);
    strmap *paths = parser_processor.get_static_handlers();

    // No paths found, fail test
    if (paths == NULL)
        FAIL() << "No paths found by get_static_handlers()";

    strmap expected_values({{"/files", "files"}});
    EXPECT_EQ(*paths, expected_values);
}

/**
 * Debugging function: prints the given string map
 */
void print_map(strmap *string_map)
{
    for ( unsigned i = 0; i < string_map->bucket_count(); ++i) {
        std::cout << "bucket #" << i << " contains:";
        for ( auto local_it = string_map->begin(i); local_it!= string_map->end(i); ++local_it )
            std::cout << " " << local_it->first << ":" << local_it->second;
        std::cout << std::endl;
    }
}
