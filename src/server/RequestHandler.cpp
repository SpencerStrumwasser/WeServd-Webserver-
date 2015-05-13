//
// Created by David Pena on 5/8/15.
//

#include "RequestHandler.h"
#include "../parser/ParserProcessor.h"
#include "FileRequestHandler.h"
#include "EchoRequestHandler.h"
#include "reply.h"

const std::string ECHO_PREFIX("/echo");
const std::string FILE_PREFIX("/static");

RequestHandler::RequestHandler(unsigned short port, strmap *locations) {
    this->port = port;
    this->locations = locations;
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
        size_t length = sock->read_some(boost::asio::buffer(data), error);
        if (error)
            throw boost::system::system_error(error);

        // Make a string from the received data
        std::string data_str(data, length);

        // Get request path
        std::string path = get_request_path(data_str);

        // Echo request
        if (ParserProcessor::value_has_prefix(path, ECHO_PREFIX)) {
            EchoRequestHandler(sock, data).launch();
            fprintf(stderr, "DEBUG: Echoing request.\n\n");
            return;
        }

        // File request
        else if (ParserProcessor::value_has_prefix(path, FILE_PREFIX)) {
            for (auto it = this->locations->begin();
                 it != this->locations->end(); ++it) {
                // Add slashes to path in string map
                std::string slash("/");
                std::string location_slash = slash + it->first + slash;
                // Check if this is the static path desired
                if (ParserProcessor::value_has_prefix(path, location_slash)) {
                    FileRequestHandler(data_str, location_slash, it->second, sock).launch();
                    return;
                }
            }

        }

        // 404 ERROR if we reach here
        reply not_found = reply::stock_reply(reply::status_type::not_found);
        using namespace boost::asio;
        write(*sock, buffer(not_found.content, not_found.content.length()));
        fprintf(stderr, "DEBUG: Unrecognized request type.\n\n");
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

/**
 * Get the path from the given request string
 */
std::string RequestHandler::get_request_path(std::string request){
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
