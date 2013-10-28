
#include "paradox_message.h"

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

using namespace paradox;
class client {
public:
	client() :m_socket(m_io_service) {
		boost::asio::ip::tcp::endpoint endpoint(
			boost::asio::ip::address::from_string("10.1.9.253"), 1777);
		m_socket.async_connect(endpoint, boost::bind(&client::handle_connect, this, boost::asio::placeholders::error));
	}

	void run() {
		m_io_service.run();
	}
private:
	void handle_connect(const boost::system::error_code& error) {
		if (error) {
			printf("connection is error\n");
			return;
		}

		printf("connection is success\n");
		test_message message;
		strcpy(message.chat, "ff");
		memcpy(m_send_buffer, &message, message.size);
		boost::asio::async_write(m_socket,
			boost::asio::buffer(m_send_buffer, message.size),
			boost::bind(&client::handle_write, this, boost::asio::placeholders::error));

		boost::asio::async_read(m_socket, 
			boost::asio::buffer(m_read_buffer, message.size),
			boost::bind(&client::handle_read, this, boost::asio::placeholders::error));
	}

	void handle_write(const boost::system::error_code& error) {
		//printf("write done\n");
	}

	void handle_read(const boost::system::error_code& error) {	
		//printf("read done\n");
// 
//  		m_socket.close();
//  		return;
		m_socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both);
		return;

		test_message message;
		strcpy(message.chat, "fft");
		memcpy(m_send_buffer, &message, message.size);
		boost::asio::async_write(m_socket,
			boost::asio::buffer(m_send_buffer, message.size),
			boost::bind(&client::handle_write, this, boost::asio::placeholders::error));

		boost::asio::async_read(m_socket, 
			boost::asio::buffer(m_read_buffer, message.size),
			boost::bind(&client::handle_read, this, boost::asio::placeholders::error));
		printf("======%s\n", m_read_buffer + sizeof(paradox_message_head));
		assert(!memcmp(&message, m_read_buffer, sizeof(message.size)));
	}
	char m_read_buffer[256];
	char m_send_buffer[256];
	boost::asio::io_service m_io_service;
	boost::asio::ip::tcp::socket m_socket;
};

int main() {
	client c;

	c.run();

	system("puase");
	return 0;
}