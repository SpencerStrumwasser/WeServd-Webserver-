#ifndef REQUESTHANDLER_H
#define REQUESTHANDLER_H

// As a reminder, here are the design decisions we voted on in class:
//
// 1. RequestHandler objects, as defined below, are long lived. That is, they
//    live for the duration of the webserver's life.
//
// 2. RequestHandler objects are configured at server startup, not per-request.
//
// 3. The request is parsed into an HTTPRequest struct by the server before
//    dispatching the request to the handler.
//
// 4. There will be one instance of an object deriving from RequestHandler
//    for each path defined in the config file.
//
// 5. Handlers will be registered in a map owned by the server that is keyed
//    by URL path prefix with value RequestHandler*. The URL prefix is defined
//    to be a first path element. For example, the prefix for:
//      /images/foo/bar
//    is
//      /images

#include <string>
#include <utility>
#include <vector>

class NginxConfig;

struct HTTPRequest {
  // The method of request (e.g. GET, HEAD, etc).
  std::string method;

  // The path (e.g. /path/to/handler/index.html).
  std::string path;

  // Parsed headers (in order).
  std::vector<std::pair<std::string, std::string>> headers;

  // The body of the request.
  std::string request_body;
};

namespace status_strings {
    const std::string ok = "HTTP/1.0 200 OK\r\n";
    const std::string not_found = "HTTP/1.0 404 Not Found\r\n\r\nFile Not Found\r\n";
}

// RequestHandler is a long lived object that is expected to be created at
// server initialization time. There should be one RequestHandler for each
// config block that will be stored in a dispatch map (see above).
//
// Expected setup:
//   RequestHandler* h = new MyDerivedHandler(...);
//   h->Configure(*config_.statements_[block_i]->child_block_.get())
//   dispatch[path] = h;
class RequestHandler {
 public:
  virtual ~RequestHandler() {}

  // Configures this request handler to handle a particular type of request.
  // It will be passed only the block of the config that pertains to this
  // handler. The config block is represented by an NginxConfig.
  //
  // This must be called before calling HandleRequest().
  virtual void Configure(const NginxConfig& child_config_block) = 0;

  // Handles and individual request. Returns the response, including the
  // return code, headers, and body.
  virtual std::string HandleRequest(const HTTPRequest& req) = 0;
};

#endif