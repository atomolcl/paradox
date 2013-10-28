#ifndef CONNECTION_POOL_H
#define CONNECTION_POOL_H

#include <boost/asio.hpp>
#include <boost/thread/mutex.hpp>

namespace paradox { class connection; }

namespace paradox {
	class connection_pool : private boost::noncopyable {
	public:
		connection_pool(boost::asio::io_service& io_service);

		~connection_pool();

		void initialize();
		
		connection* create_connection();

		void delete_connection(connection* one);

		void delete_connection(int16_t id);
	private:
		void destory();

		boost::asio::io_service& m_io_service;
		connection* m_connections;
		connection* m_free_connection;
		boost::mutex m_mutex;
	};
}
#endif