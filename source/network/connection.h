#ifndef CONNECTION_H
#define CONNECTION_H

#include <stdint.h>
#include <boost/asio.hpp>

namespace paradox {
	class connection {
	public:
		connection() : m_id(0) , m_socket(NULL) {}

		void initialize(boost::asio::io_service& io_service, int16_t id);

		void finalize(int16_t id);

		void set_id(int16_t id) { m_id = id; }

		int get_id() const { return m_id; }

		void read();
	private:

		boost::asio::ip::tcp::socket* m_socket;
		int16_t m_id;
	};
}
#endif