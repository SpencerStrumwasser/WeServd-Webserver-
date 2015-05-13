//
// Created by David Pena on 5/7/15.
//

#ifndef WESERVD_ECHOREQUESTHANDLER_H
#define WESERVD_ECHOREQUESTHANDLER_H

#include "RequestHandler.h"

class EchoRequestHandler : RequestHandler {
public:
    EchoRequestHandler(socket_ptr sock, const std::string request);
    /**
     * Respond to the request
     */
    void respond();

private:
    static std::string format_range(const std::string& format_string,
                                    const std::vector<std::string>& args);
};


#endif //WESERVD_ECHOREQUESTHANDLER_H
