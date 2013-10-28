#ifndef CONNECTION_H
#define CONNECTION_H

#include <stdint.h>
#include <boost/asio.hpp>
#include <boost/thread/mutex.hpp>

namespace paradox { class read_buffer; }

namespace paradox {
	class connection {
	public:
		enum { INVALID_ID = -1 };
		connection();
		~connection();

		void initialize(boost::asio::io_service& io_service, int16_t id);

		void finalize(int16_t id);
		
		void set_id(int16_t id) { m_id = id; }

		int16_t get_id() const { return m_id; }

		boost::asio::ip::tcp::socket& get_socket() { return *m_socket; }

		void start();

		int send(const void* s, size_t len);

		void finish_read(size_t size);
	private:		
		enum {
			READ_TEMP_BUFFER_SIZE = 256, 
			READ_BUFFER_SIZE = 256/*8 * 1024*/
		};
		enum socket_status {
			SOCKET_STATUS_INVALID    = 0,
			SOCKET_STATUS_ACTIVE      = 1,
			SOCKET_STATUS_SUSPEND  = 2
		};
		void handle_read_head(const boost::system::error_code& error, size_t bytes);
		void handle_read_body(const boost::system::error_code& error, size_t bytes);
		void post_socket_closed();
		void set_status(int8_t status) { boost::mutex::scoped_lock lock(m_mutex); m_status = status; }
		bool is_socket_active() { boost::mutex::scoped_lock lock(m_mutex); return m_status == SOCKET_STATUS_ACTIVE; }

		boost::asio::ip::tcp::socket* m_socket;
		boost::mutex m_mutex;
		//to do...
		char* m_read_temp;
		read_buffer* m_read_buffer;
		int16_t m_id;
		int8_t m_status;
	};
}
#endif