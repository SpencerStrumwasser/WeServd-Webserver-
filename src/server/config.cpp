#include "config.h"
#include <vector>
#include <map>
#include <string>
#include <utility>
#include <cstdio>

#include "EchoHandler.h"
#include "StaticHandler.h"

Config::Config(NginxConfig *conf) {
  config = conf;
}

unsigned short Config::get_port() {
  for(unsigned int i = 0; i < config->statements_.size(); i++) {
    NginxConfigStatement *stmt = config->statements_[i].get();
    std::string tok_name = stmt->tokens_[0];
    if(tok_name == "listen") {
      // The second token will then be the port number, so convert that to an
      // unsigned short and return
      return stoi(stmt->tokens_[1]);
    }
  }
  return 0;
}

std::map<std::string, RequestHandler *> Config::get_handlers() {
  std::map<std::string, RequestHandler *> map;
  for(unsigned int i = 0; i < config->statements_.size(); i++) {
    NginxConfigStatement *stmt = config->statements_[i].get();
    if(stmt->tokens_[0] == "handler") {
      std::string handler_type = stmt->tokens_[1];
      std::string path = stmt->tokens_[2];
      RequestHandler *handler;
      if(handler_type == "static") {
        handler = new StaticHandler();
      } else if(handler_type == "echo") {
        handler = new EchoHandler();
      } else {
        continue;
      }
      handler->Configure(*stmt->child_block_.get());
      map[path] = handler;
    }
  }

  return map;
}