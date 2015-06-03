#include <sstream>
#include <string>
#include <iostream>
#include <vector>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include "ProxyHandler.h"
#include "../parser/ConfigParser.h"

void ProxyHandler::Configure(const NginxConfig& child_config_block) {
    /* Get host website and port number from config file. */
    std::vector<std::shared_ptr<NginxConfigStatement>>::const_iterator iter;
    for (iter = child_config_block.statements_.begin();
         iter != child_config_block.statements_.end();
         ++iter)
    {
        std::cout << (*iter)->tokens_.size();
        if (((*iter)->tokens_.size() > 2)) {
            if((*iter)->tokens_[0] == "host") {
                host = (*iter)->tokens_[1];
                port = (*iter)->tokens_[2];
            }
        }
    }

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
    std::string request = "";
    
    // Strip handler extension.
    std::istringstream iss(req.path);
    std::string handlerExt = "";
    std::getline(iss, handlerExt, '/');
    std::getline(iss, handlerExt, '/');
    std::string path(req.path.begin() + handlerExt.size() + 1, req.path.end());
    if (path.empty()) {
        path = "/";
    } 
    request += req.method + " " + path + " HTTP/1.1\r\n\r\n";
    for (unsigned int i = 0; i < req.headers.size(); i++) {
        request += req.headers[i].first + " " + req.headers[i].second + "\r\n";
    }
    request += "\r\n" + req.request_body;
    std::cout << "Request: " << request << '\n';
    boost::system::error_code error;
    boost::asio::write(socket, boost::asio::buffer(request), error);

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
