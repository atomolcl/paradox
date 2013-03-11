#include "connection_pool.h"
#include "connection.h"

#include <stddef.h>
#include <string.h>

namespace paradox {
	connection_pool::connection_pool() : m_connections(NULL),
																m_free_connection(NULL){}

	
	connection_pool::~connection_pool() { destory(); }
	

	void connection_pool::initialize(boost::asio::io_service& io_service) {
		/*to do rewrite connections numbers*/
		int16_t connection_num = 1000;
		m_connections = new connection[connection_num];
		for (int16_t i = 0; i < connection_num; ++i)
			m_connections[i].initialize(io_service, i+1);
		
		/*last set id = invalid*/
		m_connections[connection_num - 1].set_id(-1);

		m_free_connection = &m_connections[0];
	}


	connection* connection_pool::create_connection() {
		boost::mutex::scoped_lock(m_mutex);

		connection* ret = NULL;
		if (m_free_connection) {
			ret = m_free_connection;
			int next_free = m_free_connection->get_id();
			if (next_free < 0)
				m_free_connection = NULL;
			else
				m_free_connection = m_connections + next_free;
		} else {
			//to do
		}

		return ret;
	}


	void connection_pool::delete_connection(connection* one) {
		boost::mutex::scoped_lock(m_mutex);

		if (m_free_connection)
			one->finalize((int16_t)(m_free_connection - m_connections));
		else
			one->finalize(-1);

		m_free_connection = one;
	}

	/************************praivate function***********************/
	void connection_pool::destory() {
		delete[] m_connections;
	}
}