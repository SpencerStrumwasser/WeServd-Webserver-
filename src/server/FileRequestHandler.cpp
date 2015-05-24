#include "FileRequestHandler.h"
#include "reply.h"
#include "../parser/ConfigParser.h"

#include <boost/asio.hpp>
#include <boost/algorithm/string.hpp>

#include <string>
#include <vector>
#include <sstream>
#include <fstream>

std::unordered_map<std::string, std::string> mime_types = {
  { "gif", "image/gif" },
  { "htm", "text/html" },
  { "html", "text/html" },
  { "jpg", "image/jpeg" },
  { "png", "image/png" }
};

void StaticHandler::Configure(const NginxConfig& child_config_block) {
  for(unsigned int i = 0; i < child_config_block.statements_.size(); i++) {
    NginxConfigStatement *stmt = child_config_block.statements_[i].get();
    if(stmt->tokens_[0].compare("root") == 0) {
      // this is the remote path
      root_ = stmt->tokens_[1];
      break;
    }
  }
}

std::string get_file_path(std::string path) {
  size_t found = path.find("/", 1);
  return path.substr(found, path.size() - found);
}

std::string StaticHandler::HandleRequest(const HTTPRequest& req) {
  std::string mime_type;
  std::vector<std::string> splits;
  boost::split(splits, req.path, boost::is_any_of("."));
  std::string extension = splits.back();
  if(mime_types.count(extension) == 0) {
    // we don't know a mime-type for this extension
    mime_type = "text/plain";
  } else {
    mime_type = mime_types[splits.back()];
  }

  std::string file_path = root_ + get_file_path(req.path);

  // Read file into string
  std::ifstream t(file_path.c_str());
  // Test for 404 error
  if (!t)
  {
    return status_strings::not_found;
  }
  std::stringstream buffer;
  buffer << t.rdbuf();
  std::string content = buffer.str();

  std::string status = status_strings::ok;
  std::vector<std::pair<std::string, std::string>> headers;
  headers.push_back({ "Content-Length", std::to_string(content.size()) });
  headers.push_back({ "Content-Type", mime_type });

  std::string response = status;
  // Loop over headers and concatenate them
  for(unsigned int i = 0; i < headers.size(); i++) {
    response = response + headers[i].first + ": " + headers[i].second + "\r\n";
  }
  response += "\r\n" + content;

  return response;
}

