//
// FileRequestHandler.h
// ~~~~~~~~~~~~~~~~~~~
//
// Based on WTFIAWS request_handler.h
//

#ifndef WESERVD_FILEREQUESTHANDLER_H
#define WESERVD_FILEREQUESTHANDLER_H

#include <unordered_map>
#include "RequestHandler.h"

struct reply;
struct request;

typedef std::unordered_map<std::string, std::string> strmap;

namespace status_strings {
    const std::string ok = "HTTP/1.0 200 OK\r\n";
    const std::string not_found = "HTTP/1.0 404 Not Found\r\n\r\nFile Not Found\r\n";
}

class FileRequestHandler : RequestHandler
{
public:
    /**
     * Construct with a directory containing files to be served.
     */
    FileRequestHandler(socket_ptr sock,
                       const std::string request,
                       const std::string location_name,
                       const std::string location);
    /**
     * Respond to the request
     */
    void respond();
private:
    // Location name from config file (i.e. static1, static2, etc.)
    std::string location_name;
    // Location corresponding to the location name
    std::string location;

    std::string get_response();
};

#endif // WESERVD_FILEREQUESTHANDLER_H
