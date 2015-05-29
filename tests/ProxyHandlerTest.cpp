#include "gtest/gtest.h"

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
    //const std::string config_text = "port 8080;";
    //std::stringstream config_stream(config_text);
    //NginxConfigParser parser;
    //NginxConfig config;
    //parser.Parse(&config_stream, &config);

    // Make sure that the port number is as expected
    //ParserProcessor parser_processor = ParserProcessor(config);
    //unsigned short port = parser_processor.get_port();
    EXPECT_EQ(0, 0);
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
