//Based of off WTFIAWS' code



#include "request_handler.h"

#ifndef STATICHANDLER_H
#define STATICHANDLER_H

class StaticHandler : public RequestHandler {
public:
  void Configure(const NginxConfig& child_config_block);
  std::string HandleRequest(const HTTPRequest& req);
protected:
  std::string root_;
};

#endif

