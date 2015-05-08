#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>
#include <sstream>
#include <iterator> 
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/thread/thread.hpp>
#include <thread>
#include <utility>
#include "string.h"
#include <fstream>
#include "server.hpp"
#include <boost/filesystem.hpp>
#include <array>

class FileServer: public Server{
  public: 
  	std::string run(std::string request, std::string *URLpaths);

 protected:
 	int data_length = 1024;
 	bool does_file_exist(std::string filepath);
	std::string get_file_name(std::string request);
    std::string return_file(std::string filename, std::string *URLpaths);
};

