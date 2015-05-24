
#include "../parser/ConfigParser.h"
#include <vector>
#include <map>
#include <utility>
#include <string>

#include "request_handler.h"

#ifndef CONFIG_H
#define CONFIG_H

class Config {
public:
  Config(NginxConfig *conf);
  unsigned short get_port();
  std::map<std::string, RequestHandler *> get_handlers();
protected:
  NginxConfig *config;
};

#endif