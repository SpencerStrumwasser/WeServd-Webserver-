//
// Created by David Pena on 5/12/15.
//

#ifndef WESERVD_SERVER_H
#define WESERVD_SERVER_H

#include <exception>

#include "FileRequestHandler.h"
#include "EchoRequestHandler.h"

class ExitServerException: public std::exception {
private:
    std::string message_;
public:
    ExitServerException(){}
};

class Server {
public:
    Server(unsigned short port, strmap *locations);
    /**
     * Launches the webserver
     */
    void launch();
private:
    // Port to start the webserver on
    unsigned short port;
    // Mappings from request static paths to filesystem
    strmap *locations;

    void server(boost::asio::io_service& io_service, unsigned short port);
    void session(socket_ptr sock);
    void end();

    std::string get_request_path(std::string);
};


#endif //WESERVD_SERVER_H
