#include <iostream>
#include <boost/asio.hpp>
#include "tcpserver.h"

int main()
{
	std::cout << "Test driver / entry point for tcpserver\n";
	boost::asio::io_service ios;
	gps::server::tcpserver server(ios, 1234);
	ios.run();
	server.listen();
	return 0;
}