#pragma once
#include <boost/asio.hpp>
#include <string>
#include <cstdint>
#include <deque>
namespace gps
{
	namespace server
	{
		// using
		using boost::asio::ip::tcp;
		const std::string empty_queue ="Empty queue\n";
		class tcpserver
		{
			std::deque<std::string> message_queue;
			boost::asio::io_service _io_service;
			tcp::socket _socket;
			static const size_t max_length = 128;
			const unsigned short _portnum;
			std::string read();
			void sync();
		public:
			tcpserver(const unsigned short portnum);
			~tcpserver();
			bool listen();
		};
	}
}