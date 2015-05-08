//
// Created by David Pena on 5/7/15.
//

#include "EchoRequestHandler.h"

using boost::asio::ip::tcp;

EchoRequestHandler::EchoRequestHandler(unsigned short port) {
    this->port = port;
}

/* -------------------- Public -------------------- */
void EchoRequestHandler::launch()
{
    // Launch echo server
    boost::asio::io_service io_service;

    server(io_service, this->port);
}

/* -------------------- Private -------------------- */

void EchoRequestHandler::session(socket_ptr sock)
{
    try
    {
        char data[max_length];

        boost::system::error_code error;
        sock->read_some(boost::asio::buffer(data), error);

        // Make a string to return to the request with the header (%s)
        std::string response_str_format("HTTP/1.0 200 OK\nContent-Type: text/html\n\n"
                                                "<html><body>%s</body></html>");
        // Make a string from the received data
        std::string data_str(data);
        // Make an arguments array to pass into the response string
        std::vector<std::string> args;
        args.push_back(data_str);

        // Format the response string using the request header
        std::string response_str = format_range(response_str_format, args);

        // Echo the request header
        boost::asio::write(*sock, boost::asio::buffer(response_str.c_str(),
                                                      response_str.size()));
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception in thread: " << e.what() << "\n";
    }
}

void EchoRequestHandler::server(boost::asio::io_service& io_service,
                                unsigned short port)
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

/* --- Helper Functions --- */

std::string EchoRequestHandler::format_range(const std::string& format_string,
                                             const std::vector<std::string>& args)
{
    boost::format f(format_string);
    for (std::vector<std::string>::const_iterator it = args.begin(); it != args.end(); ++it) {
        f % *it;
    }
    return f.str();
}