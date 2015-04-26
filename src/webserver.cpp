// Code based off of echo Server boost: 
// http://www.boost.org/doc/libs/1_55_0/doc/html/boost_asio/example/cpp03/
//     echo/blocking_tcp_echo_server.cpp

#include <cstdlib>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>

#include "parser/ParserProcessor.h"

static const char *server_domain = "http://localhost";

using boost::asio::ip::tcp;

typedef boost::shared_ptr<tcp::socket> socket_ptr;

void session(socket_ptr sock)
{
    try
    {
        // Send Hello World message
        std::string hello_str("HTTP/1.0 200 OK\nContent-Type: text/html\n\n"
                                      "<html><body>Hello, world!</body></html>");
        boost::asio::write(*sock, boost::asio::buffer(hello_str.c_str(),
                                                      hello_str.size()));
        
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception in thread: " << e.what() << "\n";
    }
}

void server(boost::asio::io_service& io_service, unsigned short port)
{
    // Accept incoming connections
    tcp::acceptor a(io_service, tcp::endpoint(tcp::v4(), port));
    while(true)
    {
        socket_ptr sock(new tcp::socket(io_service));
        a.accept(*sock);
        boost::thread t(boost::bind(session, sock));
    }
}

int main(int argc, char* argv[])
{
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
        fprintf(stderr, "Starting server at %s:%d\n",server_domain, port);

        // Launch echo server
        boost::asio::io_service io_service;
        server(io_service, port);
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}


