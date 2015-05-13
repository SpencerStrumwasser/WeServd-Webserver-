//
// Created by David Pena on 5/8/15.
//

#ifndef WESERVD_REQUESTHANDLER_H
#define WESERVD_REQUESTHANDLER_H

#include <boost/bind.hpp>
#include <boost/format.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>
#include <boost/algorithm/string.hpp>
#include <string>

#include "../parser/ParserProcessor.h"

const int max_length = 1024;

typedef boost::shared_ptr<boost::asio::ip::tcp::socket> socket_ptr;

class RequestHandler {
public:
    RequestHandler();
    RequestHandler(socket_ptr sock, std::string request);
    /**
     * Respond to the request
     */
    virtual void respond() {};
    /**
     * Sends the response with the specified length via the handler's socket.
     */
    void send_response(std::string response, size_t length);

protected:
    // Socket for this request handler
    socket_ptr sock;
    // Request sent to the handler
    std::string request;
};


#endif //WESERVD_REQUESTHANDLER_H
