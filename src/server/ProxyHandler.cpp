#include <sstream>
#include <string>
#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include "ProxyHandler.h"

void ProxyHandler::Configure(const NginxConfig& child_config_block) {
    host = "www.caltech.edu";
    port = "80";
}

std::string ProxyHandler::HandleRequest(const HTTPRequest& req) {
    boost::asio::io_service io_service;
    boost::asio::ip::tcp::resolver resolver(io_service);
    // Construct query using name of server and service
    boost::asio::ip::tcp::resolver::query query(host, port);
    boost::asio::ip::tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
    boost::asio::ip::tcp::socket socket(io_service);
    
    // Try connecting to server
    boost::asio::connect(socket,endpoint_iterator);

    // Send get request
    std::stringstream request_;
    request_ << req.method + " " + req.path + " HTTP/1.1\r\n";
    for (int i = 0; i < req.headers.size(); i++) {
        request_ << req.headers[i].first + " " + req.headers[i].second + "\r\n";
    }
    request_ << "\r\n" + req.request_body;
    std::cout << "Request: " << request_.str() << '\n';
    boost::system::error_code error;
    boost::asio::write(socket, boost::asio::buffer(request_.str()), error);

    std::string response = "";
    for (;;) {
        boost::array<char, 128> buf;
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
        std::string str_buffer(buf.data(), len);
        response += str_buffer;
    }
    std::cout << "Response: " << response << '\n';
    return response;
}
