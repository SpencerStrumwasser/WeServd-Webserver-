#include <string>
#include <vector>
#include <map>

#include <boost/bind.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/thread/thread.hpp>
#include <boost/asio.hpp>

#include "config.h"

#ifndef SERVER_H
#define SERVER_H

using boost::asio::ip::tcp;

typedef boost::shared_ptr<tcp::socket> sock_ptr;
const int max_length = 1024;

class Server {
public:
  // added doc_root to this
  Server(boost::asio::io_service& io_service, Config *conf);
  void run();
  std::string get_prefix(std::string full_path);
protected:
  void static session(sock_ptr sock, Server *s);
  unsigned short port_;
  boost::asio::io_service *service_;

  std::map<std::string, RequestHandler *> handlers_;

  const HTTPRequest parseRequest(std::istream &stream);

  void serve_file(sock_ptr sock, std::string file_name);

  // The handler for all incoming requests
  //request_handler request_handler_;
};

#endif
