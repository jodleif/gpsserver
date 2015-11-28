#include <iostream>
#include "tcpserver.h"

int main()
{
	std::cout << "Test driver / entry point for tcpserver\n";
	gps::server::tcpserver server(1234);
	server.listen();
	return 0;
}