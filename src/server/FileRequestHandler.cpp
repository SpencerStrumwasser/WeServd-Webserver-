//
// Created by David Pena on 5/7/15.
//
// FileRequestHandler.cpp
// ~~~~~~~~~~~~~~~~~~~
//
// Based on WTFIAWS request_handler.cpp
//

#include "FileRequestHandler.h"
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>

#include "reply.h"
#include "request.h"

std::unordered_map<std::string, std::string> mime_types = {
        { "gif", "image/gif" },
        { "htm", "text/html" },
        { "html", "text/html" },
        { "jpg", "image/jpeg" },
        { "png", "image/png" }
};

FileRequestHandler::FileRequestHandler(const std::string request,
                                       const std::string location,
                                       const std::string path,
                                       socket_ptr sock)
{
    this->request = request;
    this->path = path;
    this->location = location;
    this->sock = sock;
}

/* -------------------- Public -------------------- */

void FileRequestHandler::launch()
{

    std::string response = get_response(this->request);
    try
    {
        // Echo the request header
        boost::asio::write(*sock, boost::asio::buffer(response.c_str(),
                                                      response.size()));
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception in thread: " << e.what() << "\n";
    }
}

/* -------------------- Private -------------------- */

std::string FileRequestHandler::get_response(std::string request) {
    std::string req_type;
    std::string req_path;
    std::stringstream ss(request);

    ss >> req_type >> req_path;

    printf("DEBUG: Serving file: %s\n", req_path.c_str());

    // Remove the part corresponding to the path that says we want static
    // serving
    req_path.erase(0, location.length());
    // Now insert the base path that we want to go to
    req_path.insert(0, path);

    // Determine mime type
    std::string mime_type;
    std::vector<std::string> splits;
    boost::split(splits, req_path, boost::is_any_of("."));
    std::string extension = splits.back();
    if(mime_types.count(extension) == 0) {
        // we don't know a mime-type for this extension
        mime_type = "text/plain";
    } else {
        mime_type = mime_types[splits.back()];
    }

    // Read file into string
    std::ifstream t(req_path.c_str());
    // Test for 400 error
    if (!t)
    {
        return status_strings::not_found;
    }
    std::stringstream buffer;
    buffer << t.rdbuf();
    std::string content = buffer.str();

    // Fill out the header to be written to the connection.
    std::string status = status_strings::ok;
    std::vector<header> headers;
    headers.push_back((header){ "Content-Length", std::to_string(content.size()) });
    headers.push_back((header){ "Content-Type", mime_type }); // should be actual mime-type later
    std::string response = status;
    // Loop over headers and concatenate them
    for(unsigned int i = 0; i < headers.size(); i++) {
        response = response + headers[i].name + ": " + headers[i].value + "\r\n";
    }
    response += "\r\n" + content;

    return response;
}
