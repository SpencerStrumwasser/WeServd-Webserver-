#include <iostream>
#include <string>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

std::string make_Hello_World()
{
	return "Hello World"

}

int main()
{
	try
	{
		boost::asio::io_service io_service;
		tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), 13));
		for(;;)
		{
			tcp::socket socket(io_service);
			acceptor.accept(socket);
			std::string message = make_Hellow_World();
			boost::system::error_code ignored_error;
			boost::asio::write(socket, boost::asio::buffer(message), ignored_error);
		}
	}
	catech(std::exception& e)ls
    {
        std::cerr<<e.what() <<std::endl;
    }
	return 0;
}

