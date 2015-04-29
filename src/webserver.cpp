// Code based off of echo Server boost: 
// http://www.boost.org/doc/libs/1_55_0/doc/html/boost_asio/example/cpp03/
//     echo/blocking_tcp_echo_server.cpp

#include <cstdlib>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/format.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>

#include "parser/ParserProcessor.h"

static const char *server_domain = "http://localhost";

using boost::asio::ip::tcp;

const int max_length = 1024;

typedef boost::shared_ptr<tcp::socket> socket_ptr;

std::string format_range(const std::string& format_string, const std::vector<std::string>& args)
{
    boost::format f(format_string);
    for (std::vector<std::string>::const_iterator it = args.begin(); it != args.end(); ++it) {
        f % *it;
    }
    return f.str();
}

void session(socket_ptr sock)
{
    try
    {

            char data[max_length];

            boost::system::error_code error;
            size_t length = sock->read_some(boost::asio::buffer(data), error);

            // Make a string to return to the request with the header (%s)
            std::string response_str_format("HTTP/1.0 200 OK\nContent-Type: text/html\n\n"
                                             "<html><body>%s</body></html>");
            // Make a string from the received data
            std::string data_str(data);
            // Make an arguments array to pass into the response string
            std::vector<std::string> args;
            args.push_back(data_str);

            std::string response_str = format_range(response_str_format, args);

            boost::asio::write(*sock, boost::asio::buffer(response_str.c_str(),
                                                          response_str.size()));
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
        using namespace std;
        server(io_service, port);
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}


