#include "network_service.h"
#include "platform.h"
#include <stdlib.h>

#include <boost/bind.hpp>

namespace paradox {
	server::server() : m_thread_num(0),
		m_signals(m_io_service),
		m_acceptor(m_io_service)
	{
	}


	void server::initialize() {
		m_signals.add(SIGINT);
		m_signals.add(SIGTERM);
#if defined(SIGQUIT)
		m_signals.add(SIGQUIT);
#endif
		m_signals.async_wait(boost::bind(&server::finalize, this));
		m_thread_num = (uint8_t)(get_cpu_num() * 1.5);

		//port –¥À¿
		m_acceptor.open(boost::asio::ip::tcp::v4());
		m_acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
		boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::tcp::v4(), 1777);
		m_acceptor.bind(endpoint);
		m_acceptor.listen();
	}

	
	/********************private fuction***********************************/
	void server::finalize() {
		m_io_service.stop();
	}
}
