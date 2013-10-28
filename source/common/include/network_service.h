#ifndef NET_WORK_SERVICE_H
#define NET_WORK_SERVICE_H

#include <boost/asio.hpp>
#include <stdint.h>
#include <list>

namespace paradox {
	class connection_pool;
	class connection;
	class game_object;
	struct paradox_message_head;
	struct paradox_native_message;
}

namespace paradox {
	class network_service : private boost::noncopyable {
	public:
		network_service();
		~network_service();

		void initialize();

		void run();

		void subscribe(game_object& o);

		void post(const paradox_native_message& msg);

		void delete_connection(int16_t id);
	private:
		void start_accept();
		void finalize();
		void handle_accept(connection* new_connection, const boost::system::error_code& error);

		boost::asio::io_service m_io_service;
		boost::asio::ip::tcp::acceptor m_acceptor;
		boost::asio::signal_set m_signals;
		std::list<game_object*> m_objects;
		bool m_exit;
		connection_pool* m_pool;
		uint8_t m_thread_num;
	};
}

extern paradox::network_service g_service;
#endif