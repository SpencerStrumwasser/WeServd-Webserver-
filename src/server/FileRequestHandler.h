//
// request_handler.h
// ~~~~~~~~~~~~~~~~~~~
//
// Based on WTFIAWS request_handler.h
//

#ifndef WESERVD_FILEREQUESTHANDLER_H
#define WESERVD_FILEREQUESTHANDLER_H

#include <boost/noncopyable.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/asio.hpp>
#include <unordered_map>

struct reply;
struct request;

typedef boost::shared_ptr<boost::asio::ip::tcp::socket> socket_ptr;
typedef std::unordered_map<std::string, std::string> strmap;

namespace status_strings {
    const std::string ok = "HTTP/1.0 200 OK\r\n";
    const std::string not_found = "HTTP/1.0 404 Not Found\r\n\r\nFile Not Found\r\n";
}

/// The common handler for all incoming requests.
class FileRequestHandler : private boost::noncopyable
{
public:
    /**
     * Construct with a directory containing files to be served.
     */
    FileRequestHandler(const std::string request, const std::string location,
                       const std::string path, socket_ptr sock);
    /**
     * Get Response to the given request
     */
    std::string get_response(std::string request);

    bool path_matches(std::string req_path);

    void launch();

private:
    std::string request;
    std::string path;
    std::string location;

    // Socket to serve request to
    socket_ptr sock;
};

#endif // WESERVD_FILEREQUESTHANDLER_H