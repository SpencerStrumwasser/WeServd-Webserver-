// Code based off of echo Server boost: 
// http://www.boost.org/doc/libs/1_55_0/doc/html/boost_asio/example/cpp03/
//     echo/blocking_tcp_echo_server.cpp

#include <cstdlib>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>
#include "parser/config_parser.h"

using boost::asio::ip::tcp;

const int max_length = 1024;

typedef boost::shared_ptr<tcp::socket> socket_ptr;

void session(socket_ptr sock)
{
    try
    {
        while(true)
        {
            char data[max_length];

            boost::system::error_code error;
            size_t length = sock->read_some(boost::asio::buffer(data), error);
            if (error == boost::asio::error::eof)
                break; // Connection closed cleanly by peer.
            else if (error)
                throw boost::system::system_error(error); // Some other error.
            std::string hello_str = "<html><body>Hello, world!</body></html>";
            boost::asio::write(*sock, boost::asio::buffer(hello_str.c_str(),
                                                          hello_str.size()));
        }
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception in thread: " << e.what() << "\n";
    }
}

void server(boost::asio::io_service& io_service, unsigned short port)
{
    tcp::acceptor a(io_service, tcp::endpoint(tcp::v4(), port));
    // Accept connections
    while(true)
    {
        socket_ptr sock(new tcp::socket(io_service));
        a.accept(*sock);
        boost::thread t(boost::bind(session, sock));
    }
}

int process_statements(std::vector<std::shared_ptr<NginxConfigStatement>> statements)
{
    // Flag specifying that the next token read will be the port number
    bool next_token_port = false;

    // Number of statements to iterate through
    unsigned long num_statements = statements.size();
    for(unsigned long i = 0; i < num_statements; i++)
    {
        // Statement at the current index
        std::shared_ptr<NginxConfigStatement> cur_statement;
        cur_statement = statements.at(i);

        // Check if there are any child statements
        if (cur_statement->child_block_.get() != nullptr) {
            // Get child statements and process them recursively
            std::vector<std::shared_ptr<NginxConfigStatement>> child_statements;
            child_statements = cur_statement->child_block_->statements_;

            // Try to get the port from the child statements, if found return it
            int try_port = process_statements(child_statements);
            if (try_port != -1)
                return try_port;
        }

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

int get_port(NginxConfig *config) {
    // Get the statements from the config
    std::vector<std::shared_ptr<NginxConfigStatement>> statements;
    statements = config->statements_;
    return process_statements(statements);
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
        int port = get_port(&config);
        if (port < 0)
        {
            // TODO: move to exception raised
            fprintf(stderr, "ERROR: No port given in configuration file");
            return -1;
        }
        else if (port == 0)
        {
            // TODO: move to exception raised
            fprintf(stderr, "ERROR: port given cannot be 0.");
            return -1;
        }
        else if (port > 65536)
        {
            // TODO: move to exception raised
            fprintf(stderr, "ERROR: Port out of bounds: not in range 1-65536");
            return -1;
        }

        // Launch echo server
        boost::asio::io_service io_service;
        server(io_service, (unsigned short)port);
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}


