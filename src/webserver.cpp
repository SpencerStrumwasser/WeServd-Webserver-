// Code based off of echo Server boost: 
// http://www.boost.org/doc/libs/1_55_0/doc/html/boost_asio/example/cpp03/
//     echo/blocking_tcp_echo_server.cpp

#include <cstdlib>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>

using boost::asio::ip::tcp;

const int max_length = 1024;

typedef boost::shared_ptr<tcp::socket> socket_ptr;

void session(socket_ptr sock)
{
    try
    {
        for (;;)
        {
            char data[max_length];

            boost::system::error_code error;
            size_t length = sock->read_some(boost::asio::buffer(data), error);
            if (error == boost::asio::error::eof)
                break; // Connection closed cleanly by peer.
            else if (error)
                throw boost::system::system_error(error); // Some other error.

            boost::asio::write(*sock, boost::asio::buffer(data, length));
        }
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception in thread: " << e.what() << "\n";
    }
}

int get_port(NginxConfig *config) {

    // Flag specifying that the next token read will be the port number
    bool next_token_port = false;

    // Number of statements to iterate through
    unsigned long num_statements = config->statements_.size();
    for(unsigned long i = 0; i < num_statements; i++)
    {
        // Statement at the current index
        shared_ptr<NginxConfigStatement> cur_statement;
        cur_statement = config->statements_.at(i);

        // Number of tokens for the current statement to iterate through
        unsigned long num_tokens = cur_statement->tokens_.size();
        for(unsigned long j = 0; j < num_tokens; j++) {
            // Token at the index
            std::string cur_token = cur_statement->tokens_.at(j);
            // Last token read was the port specifier
            if (next_token_port)
                return std::stoi(cur_token);
            // Port string specifier
            if (cur_token == "port")
                next_token_port = true;
        }
    }
    return -1;
}

void server(boost::asio::io_service& io_service, unsigned int port)
{
    tcp::acceptor a(io_service, tcp::endpoint(tcp::v4(), port));
    for (;;)
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

        boost::asio::io_service io_service;

        // Get the port from the parsed configuration file
        unsigned int port = get_port(&config);
        printf("Port given is %d", port);

        using namespace std; // For atoi.
        server(io_service, port);
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
