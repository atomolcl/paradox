#ifndef NET_WORK_SERVICE_H
#define NET_WORK_SERVICE_H

#include "noncopyable.h"

#include <boost/asio.hpp>


namespace paradox {
	class server : private noncopyable {
	public:
		server();

		void initialize();
	private:
		void finalize();

		boost::asio::io_service m_io_service;
		boost::asio::ip::tcp::acceptor m_acceptor;
		boost::asio::signal_set m_signals;
		uint8_t m_thread_num;
	};
}
#endif