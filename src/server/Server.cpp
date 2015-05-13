//
// Created by David Pena on 5/12/15.
//

#include "Server.h"
#include "reply.h"

const std::string ECHO_PREFIX("/echo");
const std::string FILE_PREFIX("/static");

Server::Server(unsigned short port, strmap *locations) {
    this->port = port;
    this->locations = locations;
}

void Server::launch()
{
    // Launch server
    boost::asio::io_service io_service;
    server(io_service, this->port);
}

void Server::session(socket_ptr sock)
{
    boost::this_thread::sleep_for(boost::chrono::milliseconds(10));
    try
    {
        char data[max_length];

        // Get request data
        boost::system::error_code error;
        size_t length = sock->read_some(boost::asio::buffer(data), error);
        if (error)
            throw boost::system::system_error(error);

        // Make a string from the received data
        std::string request(data, length);
        // Get request path
        std::string path = get_request_path(request);

        // Echo request
        if (ParserProcessor::value_has_prefix(path, ECHO_PREFIX)) {
            #ifdef DEBUG
                fprintf(stderr, "DEBUG: Echoing request.\n\n");
            #endif
            EchoRequestHandler(sock, request).respond();
            return;
        }
        // File request
        else if (ParserProcessor::value_has_prefix(path, FILE_PREFIX)) {
            // Check all defined static locations
            for (auto it = this->locations->begin();
                 it != this->locations->end(); ++it) {
                // Add slashes to location name in string map
                std::string slash("/");
                std::string location_name_slash = slash + it->first + slash;
                // Check if this is the static path desired
                if (ParserProcessor::value_has_prefix(path, location_name_slash)) {
                    FileRequestHandler(sock, request, location_name_slash,
                                       it->second).respond();
                    return;
                }
            }
        }

        // 404 ERROR if we reach here
        #ifdef DEBUG
            fprintf(stderr, "DEBUG: Unrecognized request type.\n\n");
        #endif
        reply not_found = reply::stock_reply(reply::status_type::not_found);
        RequestHandler(sock, request).send_response(not_found.content,
                                                    not_found.content.length());
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception in thread: " << e.what() << "\n";
    }
}

void Server::server(boost::asio::io_service& io_service,
                    unsigned short port)
{
    using namespace boost::asio::ip;
    // Accept incoming connections
    tcp::acceptor a(io_service, tcp::endpoint(tcp::v4(), port));
    try {
        // Start a thread that checks if the user entered q to quit the
        // program in that case
        boost::thread q(boost::bind(&Server::end, this));
        while(true)
        {
            socket_ptr sock(new tcp::socket(io_service));
            a.accept(*sock);
            boost::thread t(boost::bind(&Server::session, this, sock));
        }
    }
    catch(std::exception& e) {
        printf("\n\n\nCaught exception\n\n\n");

    }
}

/**
 * Throws an ExitErrorException which should be caught to quit all
 * server threads.
 */
void Server::end()
{
    std::string quit;
    while (quit != "q")
    {
        printf("Enter q to quit the server: ");
        // Release control to server
        boost::this_thread::sleep_for(boost::chrono::milliseconds(10));
        std::cin >> quit;
    }
    // If we reach here, the user entered "q"
    throw ExitServerException();
}

/* --- Helper Functions --- */

/**
 * Get the path from the given request string
 */
std::string Server::get_request_path(std::string request){
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