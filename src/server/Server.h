//
// Created by David Pena on 5/12/15.
//

#ifndef WESERVD_SERVER_H
#define WESERVD_SERVER_H


#include "FileRequestHandler.h"
#include "EchoRequestHandler.h"

class Server {
public:
    Server(unsigned short port, strmap *locations);
    void launch();
private:
    unsigned short port;
    strmap *locations;

    void server(boost::asio::io_service& io_service, unsigned short port);
    void session(socket_ptr sock);

    std::string get_request_path(std::string);
};


#endif //WESERVD_SERVER_H
