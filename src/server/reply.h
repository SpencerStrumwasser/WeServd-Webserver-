#include <string>
#include <unordered_map>

#ifndef REPLY_H
#define REPLY_H

// For reference, see
// http://www.boost.org/doc/libs/1_51_0/doc/html/boost_asio/example/http/server/reply.cpp

namespace status_strings {
    const std::string ok = "HTTP/1.0 200 OK\r\n";
    const std::string not_found = "HTTP/1.0 404 Not Found\r\n\r\nFile Not Found\r\n";
}

#endif
