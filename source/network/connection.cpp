#include "connection.h"
#include "paradox_message.h"
#include "read_buffer.h"
#include "cookie.h"
#include "network_service.h"

#include <stddef.h>
#include <boost/bind.hpp>

namespace paradox {
	connection::connection()
		: m_id(INVALID_ID), 
		m_socket(NULL),
		m_status(SOCKET_STATUS_INVALID)
	{
		m_read_temp = new char[READ_BUFFER_SIZE];
		m_read_buffer = new read_buffer(READ_BUFFER_SIZE);
	}

	connection::~connection() {
		delete[] m_read_temp;
		delete m_read_buffer;
	}

	void connection::initialize(boost::asio::io_service& io_service, int16_t id) {
		paradox_assert(!m_socket);
		set_id(id);
		m_socket = new boost::asio::ip::tcp::socket(io_service);
		//memset(m_read_temp, 0x20, READ_BUFFER_SIZE);
		m_read_buffer->initialize();
	}


	void connection::finalize(int16_t id) {
		set_id(id);
		m_socket->close(); /*todo*/
		delete m_socket;
		m_socket = NULL;
		/*m_status = SOCKET_STATUS_INVALID;*/
		set_status(SOCKET_STATUS_INVALID);
	}

	void connection::start() {
		/*m_status = SOCKET_STATUS_ACTIVE;*/
		set_status(SOCKET_STATUS_ACTIVE);
		boost::asio::async_read(*m_socket, 
		 	boost::asio::buffer(m_read_temp, sizeof(paradox_message_head)), 
		 	boost::bind(&connection::handle_read_head,
								this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
	}


	int connection::send(const void* s, size_t len) {
		if (is_socket_active())
			return boost::asio::write(*m_socket, boost::asio::buffer(s, len));
		return -1;
	}


	void connection::finish_read(size_t size) {
		m_read_buffer->finish_read(size);
	}

	/**********************private function***************************/
	void connection::handle_read_head(const boost::system::error_code& error, size_t bytes) {
		if (!error) {
			paradox_assert(bytes == sizeof(paradox_message_head));
			//printf("---%s\n", m_read_temp);
			uint16_t message_size = *(uint16_t*)m_read_temp;
			m_read_buffer->write(m_read_temp, bytes);

			boost::asio::async_read(*m_socket, 
				boost::asio::buffer(m_read_temp/* + bytes*/, message_size - sizeof(paradox_message_head)), 
				boost::bind(&connection::handle_read_body,
				this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
		} else {
			//to do...
			post_socket_closed();
		}
	}


	void connection::handle_read_body(const boost::system::error_code& error, size_t bytes) {
		if (!error) {
			paradox_assert(bytes == sizeof(test_message) - sizeof(paradox_message_head));
			//printf("===%s\n", m_read_temp/* + sizeof(paradox_message_head)*/);
			m_read_buffer->write(m_read_temp, bytes);
			//printf("+++%s\n", (char*)m_read_buffer->begin_read() + sizeof(paradox_message_head));
			paradox_message_head* msg = m_read_buffer->begin_read();
			size_t t = send(msg, msg->size);
			m_read_buffer->finish_read(msg->size);
			
			/*next message*/
			boost::asio::async_read(*m_socket, 
				boost::asio::buffer(m_read_temp, sizeof(paradox_message_head)), 
				boost::bind(&connection::handle_read_head,
				this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
		} else {
			post_socket_closed();
		}
	}


	void connection::post_socket_closed() {
		/*m_status = SOCKET_STATUS_SUSPEND;*/
		set_status(SOCKET_STATUS_SUSPEND);
		pnmt_player_disconnect msg;
		msg.connection_id = m_id;
		g_service.post(msg);
	}
}