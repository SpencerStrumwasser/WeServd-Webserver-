//
// Created by David Pena on 5/7/15.
//

#ifndef WESERVD_ECHOREQUESTHANDLER_H
#define WESERVD_ECHOREQUESTHANDLER_H

#include "RequestHandler.h"

class EchoRequestHandler {
public:
    EchoRequestHandler(socket_ptr sock, std::string echo);
    void launch();

private:
    socket_ptr sock;
    std::string echo;

    void server(boost::asio::io_service& io_service, unsigned short port);
    static void session(socket_ptr sock);
    static std::string format_range(const std::string& format_string,
                                    const std::vector<std::string>& args);
};


#endif //WESERVD_ECHOREQUESTHANDLER_H
