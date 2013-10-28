#include "connection_pool.h"
#include "connection.h"

#include <stddef.h>
#include <string.h>
#include "cookie.h"

namespace paradox {
	connection_pool::connection_pool(boost::asio::io_service& io_service) 
		: m_connections(NULL),
		  m_free_connection(NULL),
		  m_io_service(io_service)
	{}

	
	connection_pool::~connection_pool() { destory(); }
	

	void connection_pool::initialize() {
		m_connections = new connection[MAX_CONNECTIONS_NUM];
		for (int16_t i = 0; i < MAX_CONNECTIONS_NUM - 1; ++i)
			m_connections[i].set_id(i+1);
		
		m_free_connection = &m_connections[0];
	}


	connection* connection_pool::create_connection() {
		boost::mutex::scoped_lock lock(m_mutex);
		connection* ret = NULL;

		if (m_free_connection) {
			ret = m_free_connection;
			int16_t next_free = m_free_connection->get_id();
			if (next_free == connection::INVALID_ID)
				m_free_connection = NULL;
			else
				m_free_connection = m_connections + next_free;

			paradox_assert(ret);
			ret->initialize(m_io_service, (int16_t)(ret - m_connections));
		} else {
			//to do
		}

		return ret;
	}


	void connection_pool::delete_connection(connection* one) {
		boost::mutex::scoped_lock lock(m_mutex);

		if (m_free_connection)
			one->finalize((int16_t)(m_free_connection - m_connections));
		else
			one->finalize(connection::INVALID_ID);

		m_free_connection = one;
	}


	void connection_pool::delete_connection(int16_t id) {
		delete_connection(&m_connections[id]);
	}

	/************************praivate function***********************/
	void connection_pool::destory() {
		delete[] m_connections;
	}
}