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
    RequestHandler(unsigned short port, strmap *locations);
    void launch();

private:
    unsigned short port;
    strmap *locations;

    void server(boost::asio::io_service& io_service, unsigned short port);
    void session(socket_ptr sock);

    std::string get_request_path(std::string);
    bool path_matches(std::string path, std::string req_path);
};


#endif //WESERVD_REQUESTHANDLER_H
