// Code based off of echo Server boost: 
// http://www.boost.org/doc/libs/1_55_0/doc/html/boost_asio/example/cpp03/
//     echo/blocking_tcp_echo_server.cpp

#include <cstdlib>
#include <iostream>

#include "parser/ParserProcessor.h"
#include "server/FileRequestHandler.h"
#include "server/EchoRequestHandler.h"

static const char *server_domain = "http://localhost";

int main(int argc, char* argv[])
{
    using namespace std;
    try
    {
        if (argc != 2) {
            printf("Usage: ./webserver <config file>\n");
            return 1;
        }

        // Parse the given configuration file
        NginxConfigParser config_parser;
        NginxConfig config;
        config_parser.Parse(argv[1], &config);

        // Get the port from the parsed configuration file
        ParserProcessor parser_processor = ParserProcessor(config);
        unsigned short port = parser_processor.get_port();

        // Print server address
        fprintf(stderr, "Starting server at %s:%d\n", server_domain, port);
        
        // TODO: move to option in config file
        bool echo = false;
        // Create request handler depending on the option (echo or file handler)
        if (!echo){
            strmap *paths = parser_processor.get_paths();
            for (auto it = paths->begin(); it != paths->end(); ++it){
                FileRequestHandler fileRequestHandler(it->second);
            } 
        }
        else {
            EchoRequestHandler requestHandler(port);
            requestHandler.launch();
        }
    }
    catch (std::exception& e) {
        cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}


