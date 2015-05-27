#include <string>

#include <boost/bind.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/thread/thread.hpp>
#include <boost/asio.hpp>
#include <boost/algorithm/string.hpp>

#include "Server.h"
#include "header.h"

static const char *server_domain = "http://localhost";

using boost::asio::ip::tcp;

Server::Server(boost::asio::io_service& io_service, Config *conf) {
    service_ = &io_service;
    port_ = conf->get_port();
    handlers_ = conf->get_handlers();
}

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

void Server::run() {
  fprintf(stderr, "Starting server at %s:%d\n", server_domain, port_);
  tcp::acceptor a(*service_, tcp::endpoint(tcp::v4(), port_));
  boost::thread q(boost::bind(&Server::end, this));
  while(true) {
    sock_ptr sock(new tcp::socket(*service_));
    a.accept(*sock);
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
        // Read request into buffer
        char req_buf[max_length];

        boost::system::error_code error;
        size_t length = sock->read_some(boost::asio::buffer(req_buf), error);
        if (error)
          throw boost::system::system_error(error);

        std::stringstream stream(std::string(req_buf, length));

        // Create HTTP request from read request
        const HTTPRequest request = s->parseRequest(stream);
        std::string prefix = s->get_prefix(request.path);

        using namespace boost::asio;
        if(s->handlers_.count(prefix) > 0) {
            std::string response = s->handlers_[prefix]->HandleRequest(request);
            write(*sock, buffer(response, response.size()));
        } else {
            write(*sock, buffer(status_strings::not_found,
                                status_strings::not_found.size()));
        }

        // 404 ERROR if we reach here
        #ifdef DEBUG
            fprintf(stderr, "DEBUG: Unrecognized request type.\n\n");
        #endif

    }
    catch (std::exception& e) {
        std::cerr << "Exception in thread: " << e.what() << "\n";
    }
}

/**
 * Throws an ExitErrorException which should be caught to quit all
 * server threads.
 */
void Server::end()
{
    std::string quit;
    while (quit != "q")
    {
        printf("Enter q to quit the server: ");
        // Release control to server
        boost::this_thread::sleep_for(boost::chrono::milliseconds(10));
        std::cin >> quit;
    }
    // If we reach here, the user entered "q"
    throw ExitServerException();
}