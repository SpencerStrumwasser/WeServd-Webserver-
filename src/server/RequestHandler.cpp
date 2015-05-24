//
// Created by David Pena on 5/8/15.
//

#include "RequestHandler.h"
#include "FileRequestHandler.h"
#include "EchoRequestHandler.h"



/* -------------------- Public --------------------- */

RequestHandler::RequestHandler()
{

    this->sock = nullptr;
    this->request = nullptr;
}


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




