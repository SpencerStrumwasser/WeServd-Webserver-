//Based of off WTFIAWS' code



#include "request_handler.h"

#ifndef ECHOHANDLER_H
#define ECHOHANDLER_H

class EchoHandler : public RequestHandler {
public:
  void Configure(const NginxConfig& child_config_block);
  std::string HandleRequest(const HTTPRequest& req);
};

#endif

