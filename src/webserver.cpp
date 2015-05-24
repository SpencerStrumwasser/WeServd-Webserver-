
#include <ctime>
#include <iostream>
#include <string>

#include <boost/bind.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/thread/thread.hpp>
#include <boost/asio.hpp>

#include "server/Server.h"

using boost::asio::ip::tcp;

NginxConfig get_config(char *filename) {
    NginxConfigParser parser;
    NginxConfig out_config;

    // Should probably care about success value
    parser.Parse(filename, &out_config);

    return out_config;
}

int main(int argc, char* argv[])
{
  try
  {
    if (argc != 2)
    {
      std::cerr << "Usage: webserver <config>\n";
      return 1;
    }

    NginxConfig conf = get_config(argv[1]);
    Config *server_conf = new Config(&conf);

    boost::asio::io_service io_service;

    Server s(io_service, server_conf);

    s.run();
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}

