#include <boost/array.hpp>
#include <boost/asio.hpp>
#include "request_handler.h"

#ifndef PROXY_HANDLER_H
#define PROXY_HANDLER_H

class ProxyHandler : public RequestHandler {
public:
  void Configure(const NginxConfig& child_config_block);
  std::string HandleRequest(const HTTPRequest& req);
protected:
  std::string host;
  std::string port;
};

#endif // PROXY_HANDLER_H

