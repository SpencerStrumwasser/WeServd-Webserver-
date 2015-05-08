//
// request_handler.hpp
// ~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2013 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef WESERVD_FILEREQUESTHANDLER_H
#define WESERVD_FILEREQUESTHANDLER_H

#include <boost/noncopyable.hpp>
#include <boost/asio.hpp>
#include <boost/smart_ptr.hpp>
#include <string>

struct reply;
struct request;

typedef boost::shared_ptr<boost::asio::ip::tcp::socket> socket_ptr;

/// The common handler for all incoming requests.
class FileRequestHandler : private boost::noncopyable
{
public:
    /// Construct with a directory containing files to be served.
    FileRequestHandler(const std::string& doc_root, socket_ptr sock);

    /// Handle a request and produce a reply.
    void handle_request(const request& req, reply& rep);

    void launch();

private:
    /// The directory containing the files to be served.
    std::string doc_root_;
    // Socket to serve request to
    socket_ptr sock;

    /// Perform URL-decoding on a string. Returns false if the encoding was
    /// invalid.
    static bool url_decode(const std::string& in, std::string& out);
};

#endif // WESERVD_FILEREQUESTHANDLER_H