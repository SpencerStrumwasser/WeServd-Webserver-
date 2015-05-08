//
// Created by David Pena on 5/7/15.
//

#ifndef WESERVD_ECHOREQUESTHANDLER_H
#define WESERVD_ECHOREQUESTHANDLER_H

#include <boost/bind.hpp>
#include <boost/format.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>

const int max_length = 1024;

typedef boost::shared_ptr<boost::asio::ip::tcp::socket> socket_ptr;

class EchoRequestHandler {
public:
    EchoRequestHandler(unsigned short port);
    void launch();

private:
    unsigned int port;
    void server(boost::asio::io_service& io_service, unsigned short port);
    static void session(socket_ptr sock);
    static std::string format_range(const std::string& format_string,
                                    const std::vector<std::string>& args);
};


#endif //WESERVD_ECHOREQUESTHANDLER_H
