#include <string>

#include <boost/bind.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/thread/thread.hpp>
#include <boost/asio.hpp>
#include <boost/algorithm/string.hpp>

#include "Server.h"
#include "header.h"


using boost::asio::ip::tcp;

std::string Server::get_prefix(std::string full_path) {
  if(full_path.at(0) != '/') {
    return full_path;
  } else {
    size_t found = full_path.find("/", 1);
    if(found != std::string::npos) {
      return full_path.substr(0, found);
    } else {
      return full_path;
    }
  }
}

Server::Server(boost::asio::io_service& io_service, Config *conf) {
  service_ = &io_service;
  port_ = conf->get_port();
  handlers_ = conf->get_handlers();
}

void Server::run() {
  printf("Server running on port %d...\n", port_);
  tcp::acceptor a(*service_, tcp::endpoint(tcp::v4(), port_));
  while(true) {
    sock_ptr sock(new tcp::socket(*service_));
    a.accept(*sock);
    // Add echo_paths and static_paths to this?
    boost::thread t(boost::bind(&Server::session, sock, this));
  }
}

const HTTPRequest Server::parseRequest(std::istream &stream) {
  HTTPRequest request;
  int content_length;
  std::string s;

  stream >> request.method;
  stream >> request.path;
  stream >> s; // Get rid of the HTTP version

  std::string header_name, header_value;
  stream.ignore(1, '\n'); // don't care about newlines
  while(stream.peek() != 13) { // while next character is not carriage return
    std::pair<std::string, std::string> header;
    getline(stream, header.first, ':');
    stream.ignore(1, ' '); // ignore whitespace
    getline(stream, header.second, '\r');
    stream.ignore(1, '\n'); // don't care about newlines
    if(header.first == "Content-Length") {
      content_length = stoi(header_value);
    }
    request.headers.push_back(header);
  }
  stream.ignore(2, '\n'); // Get rid of empty lines at the end
  if (content_length != 0) {
    request.request_body.resize(content_length);
    stream.read(&request.request_body[0], content_length);
  }
  return request;
}

void Server::session(sock_ptr sock, Server *s) {
  try {
    // call process_request() on the request sent in
    char req_buf[max_length];

    boost::system::error_code error;
    size_t length = sock->read_some(boost::asio::buffer(req_buf), error);
    if (error)
      throw boost::system::system_error(error);

    std::stringstream stream(std::string(req_buf, length));

    const HTTPRequest request = s->parseRequest(stream);

    std::string prefix = s->get_prefix(request.path);

    if(s->handlers_.count(prefix) > 0) {
      std::string response = s->handlers_[prefix]->HandleRequest(request);
      boost::asio::write(*sock, boost::asio::buffer(response, response.size()));
    } else {
      boost::asio::write(*sock, boost::asio::buffer(status_strings::not_found, status_strings::not_found.size()));
    }
  }
  catch (std::exception& e) {
    std::cerr << "Exception in thread: " << e.what() << "\n";
  }
}
