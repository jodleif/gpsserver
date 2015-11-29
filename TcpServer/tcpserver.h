#pragma once
#include <boost/asio.hpp>
#include <thread>
#include <mutex>
#include <string>
#include <cstdint>
#include <deque>
#include "uart/cppwrapper.h"
namespace gps
{
	namespace server
	{
		// using
		using boost::asio::ip::tcp;
		using gps::c_link::uart_conn;

		const std::string empty_queue ="Empty queue\n";
		class tcpserver
		{
			boost::asio::io_service _io_service;
			uart_conn gps_connection;
			std::mutex lock;
			std::condition_variable cv;
			std::deque<std::string> message_queue;
			static const size_t max_length = 128;
			const unsigned short _portnum;
			bool connected = false;
			std::string read(tcp::socket& _socket);
			void sync();
		public:
			tcpserver(const unsigned short portnum);
			~tcpserver();
			bool listen();
		};
	}
}