//
// Created by David Pena on 5/8/15.
//

#include "RequestHandler.h"
#include "../parser/ParserProcessor.h"
#include "FileRequestHandler.h"
#include "EchoRequestHandler.h"

const std::string ECHO_PREFIX("/echo");
const std::string FILE_PREFIX("/static");

RequestHandler::RequestHandler(unsigned short port) {
    this->port = port;
}

/* -------------------- Public -------------------- */
void RequestHandler::launch()
{
    // Launch echo server
    boost::asio::io_service io_service;

    server(io_service, this->port);
}

/* -------------------- Private -------------------- */

void RequestHandler::session(socket_ptr sock)
{
    try
    {
        char data[max_length];

        // Get request data
        boost::system::error_code error;
        sock->read_some(boost::asio::buffer(data), error);

        // Make a string from the received data
        std::string data_str(data);

        // Get request path
        std::string path = get_request_path(data_str);

        // Get whether it's an echo or static request
        if (ParserProcessor::value_has_prefix(path, ECHO_PREFIX)) {
            EchoRequestHandler(sock, data).launch();
            fprintf(stderr, "DEBUG: Echoing request.\n\n");
        }
        else if (ParserProcessor::value_has_prefix(path, FILE_PREFIX))
            FileRequestHandler(path, sock).launch();
        // Echo requests by default
        else {
            EchoRequestHandler(sock, data).launch();
            fprintf(stderr, "DEBUG: Unrecognized request type.\n\n");
        }

    }
    catch (std::exception& e)
    {
        std::cerr << "Exception in thread: " << e.what() << "\n";
    }
}

void RequestHandler::server(boost::asio::io_service& io_service,
                            unsigned short port)
{
    using namespace boost::asio::ip;
    // Accept incoming connections
    tcp::acceptor a(io_service, tcp::endpoint(tcp::v4(), port));
    while(true)
    {
        socket_ptr sock(new tcp::socket(io_service));
        a.accept(*sock);
        boost::thread t(boost::bind(&RequestHandler::session, this, sock));
    }
}


/* --- Helper Functions --- */

std::string RequestHandler::format_range(const std::string& format_string,
                                         const std::vector<std::string>& args)
{
    boost::format f(format_string);
    for (std::vector<std::string>::const_iterator it = args.begin();
         it != args.end(); ++it) {
        f % *it;
    }
    return f.str();
}

std::string RequestHandler::get_request_path(std::string request) {
    // Delimiters that will be between the request path
    std::string start_delimiter("GET");
    std::string end_delimeter("HTTP");
    // Get delimeter locations
    unsigned long first = request.find(start_delimiter) + start_delimiter.length();
    unsigned long last = request.find(end_delimeter);
    // Get path from request
    std::string path = request.substr(first, last - first);
    boost::algorithm::trim(path);
    return path;
}