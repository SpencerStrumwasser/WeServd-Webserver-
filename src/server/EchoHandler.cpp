//Based of off WTFIAWS' code



#include "EchoHandler.h"

#include <boost/asio.hpp>

void EchoHandler::Configure(const NginxConfig& child_config_block) {
  return;
}

std::string EchoHandler::HandleRequest(const HTTPRequest& req) {
  #ifdef DEBUG
    printf("Echoing request\n");
  #endif
  std::string result = status_strings::ok;
  result += "\n";
  result += req.method + " " + req.path + " HTTP/1.1\r\n";
  for(unsigned int i = 0; i < req.headers.size(); i++) {
    result += req.headers[i].first + ": " + req.headers[i].second + "\r\n";
  }
  result += "\n\n";
  result += req.request_body;
  return result;
}

