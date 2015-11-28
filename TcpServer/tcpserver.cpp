#include "tcpserver.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <cassert>

std::string gps::server::tcpserver::read(tcp::socket& _socket)
{
	std::string result;
	char data[max_length];
	boost::system::error_code error;
	std::cout << "BLOCKING READ!!!!!\n";
	size_t length = _socket.read_some(boost::asio::buffer(data), error);
	std::cout << "BLOCKING READ!!!!! done\n";
	if (error == boost::asio::error::eof)
		return result;
	if (error)
		throw boost::system::system_error(error);
	result.reserve(length);
	assert(length >= 0);
	for (size_t i{ 0 };i < length;++i) {
		result.push_back(data[i]);
	}
	return result;
}

void gps::server::tcpserver::sync()
{
	std::string res;
	char counter{ 0 };
	for (;;) {
		res = gps_connection.read();
		if (res.length() == 0) break;
		++counter;
		if (counter == 10) break;
		message_queue.push_back(res);
	}
}

gps::server::tcpserver::tcpserver(const unsigned short portnum) : _portnum(portnum)
{
}

gps::server::tcpserver::~tcpserver()
{

}

bool gps::server::tcpserver::listen()
{
	try {

		tcp::acceptor _acceptor(_io_service, tcp::endpoint(tcp::v4(), _portnum));
		tcp::socket _socket(_io_service);
		_acceptor.accept(_socket);
		
		//auto from_client = read(_socket);
		//if (from_client != "OK") return false;
		for (;;) {
			// READ DATA FROM GPS
			sync();
			std::this_thread::sleep_for(std::chrono::seconds(5));
			// Empty queue
			if (!message_queue.empty()) {
				while (!message_queue.empty()) {
					auto msg = message_queue.front();
					boost::asio::write(_socket, boost::asio::buffer(msg, msg.length()));
					auto from_client = read(_socket);
					std::cout << "Receiv: " << from_client << "\n";
					if (from_client != "OK") return false;
					message_queue.pop_front();
				}
			} else {
				boost::asio::write(_socket, boost::asio::buffer(empty_queue, empty_queue.length()));
			}
		}
	} catch (std::exception e) {
		std::cout << "[listen] Error!\n" << e.what() << "\n";
		return true;
	}
}
