//
// Created by David Pena on 5/8/15.
//

#include "RequestHandler.h"
#include "FileRequestHandler.h"
#include "EchoRequestHandler.h"
#include "reply.h"



/* -------------------- Public --------------------- */

RequestHandler::RequestHandler()
{
<<<<<<< HEAD
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
=======
    this->sock = nullptr;
    this->request = nullptr;
}
>>>>>>> 2e886aba5b528f4e25f1fe35280d8616940ecc68

RequestHandler::RequestHandler(socket_ptr sock, std::string request) {
    this->sock = sock;
    this->request = request;
}

void RequestHandler::send_response(std::string response, size_t length)
{
    // Echo the request header
    boost::asio::write(*sock, boost::asio::buffer(response.c_str(),
                                                  length));
}




