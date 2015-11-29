#include "tcpserver.h"
#include <iostream>
#include <chrono>
#include <boost/asio/write.hpp>
/*
std::string gps::server::tcpserver::read(tcp::socket& _socket)
{
	std::string result;
	char data[max_length];
	boost::system::error_code error;
	size_t length = _socket.read_some(boost::asio::buffer(data), error);
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
*/

void gps::server::tcpserver::start_thread()
{
	t1 = std::thread( [this] {
		std::string res;
		for (;;) {
			if (connected) break;
			lock.lock();
			res = gps_connection.read();
			if (message_queue.size() <= 1024u) {
				message_queue.push_back(res);
				lock.unlock();
			} else {
				lock.unlock();
				std::cout << "buffer full.. sleeeep\n";
				std::this_thread::sleep_for(std::chrono::seconds(100));
			}
			std::this_thread::sleep_for(std::chrono::seconds(1));
		}

	} );
}

/*

*/
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
	start_thread();
}

gps::server::tcpserver::~tcpserver()
{
}

bool gps::server::tcpserver::listen()
{
	try {

		tcp::acceptor _acceptor(_io_service, tcp::endpoint(tcp::v4(), _portnum));
		tcp::socket _socket(_io_service);

		_acceptor.accept(_socket); // Blocks here
		while (!lock.try_lock());
		connected = true;
		lock.unlock();
		t1.join();

		for (;;) {
			// READ DATA FROM GPS
			sync();
			std::this_thread::sleep_for(std::chrono::seconds(5));
			// Empty queue
			while (!message_queue.empty()) {
				auto msg = message_queue.front();
				boost::asio::write(_socket, boost::asio::buffer(msg.data(), msg.length()));
				message_queue.pop_front();
			}

		}
	} catch (std::exception e) {
		std::cout << "[listen] Error!\n" << e.what() << "\n";
		return true;
	}
}
