#pragma once
#include <boost/asio/io_service.hpp>
#include <boost/asio/basic_socket_acceptor.hpp>
#include <boost/asio/basic_socket.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <mutex>
#include <string>
#include <thread>
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
			std::thread t1;
			std::mutex lock;
			std::deque<std::string> message_queue;
			static const size_t max_length = 128;
			const unsigned short _portnum;
			bool connected = false;
			//std::string read(tcp::socket& _socket);
			void start_thread();
			void sync();
		public:
			tcpserver(const unsigned short portnum);
			~tcpserver();
			bool listen();
		};
	}
}
