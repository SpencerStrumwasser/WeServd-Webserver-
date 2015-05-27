//Based of off WTFIAWS' code


#include "request_handler.h"
#include "reply.h"

#ifndef NOTFOUNDHANDLER_H
#define NOTFOUNDHANDLER_H

// With many thanks to DreamTeamMinusSpencer!
class NotFoundHandler : public RequestHandler {
 public:
  void Configure(const NginxConfig& child_config_block);
  std::string HandleRequest(const HTTPRequest& req);
};

#endif