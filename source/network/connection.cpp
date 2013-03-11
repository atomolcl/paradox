#include "connection.h"

namespace paradox {
	void connection::initialize(boost::asio::io_service& io_service, int16_t id) {
		set_id(id);
		m_socket = new boost::asio::ip::tcp::socket(io_service);
	}


	void connection::finalize(int16_t id) {
		set_id(id);
		delete m_socket;
		m_socket = NULL;
	}
}