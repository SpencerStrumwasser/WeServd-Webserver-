#include <sstream>
#include <string>
#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include "ProxyHandler.h"

void ProxyHandler::Configure(const NginxConfig& child_config_block) {
}

std::string ProxyHandler::HandleRequest(const HTTPRequest& req) {
    boost::asio::io_service io_service;
    boost::asio::ip::tcp::resolver resolver(io_service);
    // Construct query using name of server and service
    boost::asio::ip::tcp::resolver::query query("localhost", "1080");
    boost::asio::ip::tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
    boost::asio::ip::tcp::socket socket(io_service);
    // Try connecting to server
    boost::asio::connect(socket,endpoint_iterator);
    for (;;)
    {
        boost::array<char, 128> buf;
        boost::system::error_code error;
        // Send get request
        std::stringstream request_;
        request_ << "GET / HTTP/1.1\r\n";
        request_ << "\r\n";
        boost::asio::write(socket, boost::asio::buffer(request_.str()), error);
        // Read reply from server
        size_t len = socket.read_some(boost::asio::buffer(buf), error);
        if (error == boost::asio::error::eof)
        {
            break;
        }
        else if (error)
        {
            throw boost::system::system_error(error);
        }
        std::cout.write(buf.data(), len);
    }
    return "";
}
