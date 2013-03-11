#ifndef CONNECTION_POOL_H
#define CONNECTION_POOL_H

#include "noncopyable.h"

#include <boost/asio.hpp>
#include <boost/thread/mutex.hpp>

namespace paradox { class connection; }

namespace paradox {
	class connection_pool : private noncopyable {
	public:
		connection_pool();

		~connection_pool();

		void initialize(boost::asio::io_service& io_service);
		
		connection* create_connection();

		void delete_connection(connection* one);
	private:
		void destory();

		connection* m_connections;
		connection* m_free_connection;
		boost::mutex m_mutex;
	};
}
#endif