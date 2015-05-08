//
// request_handler.hpp
// ~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2013 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef WESERVD_REQUESTHANDLER_H
#define WESERVD_REQUESTHANDLER_H

#include <string>
#include <boost/noncopyable.hpp>

struct reply;
struct request;

/// The common handler for all incoming requests.
class RequestHandler: private boost::noncopyable
{
public:
    /// Construct with a directory containing files to be served.
    explicit RequestHandler(const std::string& doc_root);

    /// Handle a request and produce a reply.
    void handle_request(const request& req, reply& rep);

private:
    /// The directory containing the files to be served.
    std::string doc_root_;

    /// Perform URL-decoding on a string. Returns false if the encoding was
    /// invalid.
    static bool url_decode(const std::string& in, std::string& out);
};

#endif // HTTP_REQUEST_HANDLER_HPP