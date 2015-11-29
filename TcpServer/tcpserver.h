#pragma once
#include <boost/config.hpp>
#include <boost/asio.hpp>
#include <mutex>
#include <string>
#include <deque>
#include "uart/cppwrapper.h"
#ifndef BOOST_NO_CXX11_EXTERN_TEMPLATE
	extern template class boost::asio::basic_stream_socket<boost::asio::ip::tcp>;
	extern template class boost::asio::basic_socket_acceptor<boost::asio::ip::tcp>;
#endif
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