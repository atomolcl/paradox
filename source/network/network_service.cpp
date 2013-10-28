#include "network_service.h"
#include "connection.h"
#include "connection_pool.h"
#include "platform.h"
#include "game_object.h"
#include "cookie.h"

#include <boost/bind.hpp>
#include <boost/thread/thread.hpp>
#include <boost/shared_ptr.hpp>
#include <vector>
#include <stdlib.h>
#include <stdio.h>

paradox::network_service g_service;

namespace paradox {
	network_service::network_service() : m_thread_num(0),
		m_signals(m_io_service),
		m_acceptor(m_io_service),
		m_exit(false)
	{
		m_pool = new connection_pool(m_io_service);
	}

	network_service::~network_service() {
		delete m_pool;
	}

	void network_service::initialize() {
		m_signals.add(SIGINT);
		m_signals.add(SIGTERM);
#if defined(SIGQUIT)
		m_signals.add(SIGQUIT);
#endif
		m_signals.async_wait(boost::bind(&network_service::finalize, this));
		m_thread_num = (uint8_t)(paradox_get_cpu_num() * 2);

		//todo...port hard code
		m_acceptor.open(boost::asio::ip::tcp::v4());
		m_acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
		boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::tcp::v4(), 1777);
		m_acceptor.bind(endpoint);
		m_acceptor.listen();

		m_pool->initialize();
		start_accept();
	}


	void network_service::run() {
		std::vector<boost::shared_ptr<boost::thread> > threads;
		for (uint8_t i = 0; i < m_thread_num; ++i) {
			boost::shared_ptr<boost::thread> thread(new boost::thread(
				boost::bind(&boost::asio::io_service::run, &m_io_service)));
			threads.push_back(thread);
		}

		uint32_t diff, last_sleep_time = 0;
		const uint32_t sleep_time_constant = 50;
		world_timer::set_last_tick(paradox_get_tick());
		std::list<game_object*>::iterator it;
		while (!m_exit) {
			//to do...
			diff = world_timer::diff();

			it = m_objects.begin();
			for (it; it != m_objects.end(); ++it)
				(*it)->tick(diff);			

			if (diff <= sleep_time_constant + last_sleep_time)
				last_sleep_time = sleep_time_constant + last_sleep_time - diff;
			else
				last_sleep_time = 0;			
			paradox_sleep(last_sleep_time);
		}

		for (size_t i = 0; i < threads.size(); ++i)
			threads[i]->join();
	}


	void network_service::subscribe(game_object& o) {
		m_objects.push_back(&o);
	}


	void network_service::post(const paradox_native_message& msg) {
		std::list<game_object*>::iterator it = m_objects.begin();
		for (it; it != m_objects.end(); ++it)
			(*it)->post(msg);
	}


	void network_service::delete_connection(int16_t id) {
		m_pool->delete_connection(id);
	}
	
	/********************private fuction***********************************/
	void network_service::finalize() {
		m_io_service.stop();
		m_exit = true;
	}


	void network_service::start_accept() {
		connection* new_connection = m_pool->create_connection();
		if (!new_connection) {
			// todo
			return;
		}

		m_acceptor.async_accept(new_connection->get_socket(), 
			boost::bind(&network_service::handle_accept,
								this, new_connection,	boost::asio::placeholders::error));
	}


	void network_service::handle_accept(connection* new_connection,
		const boost::system::error_code& error) {
			if (!error) {
				pnmt_player_connect msg;
				msg.connection_id = new_connection->get_id();
				post(msg);
				new_connection->start();
			} else 
				void(0);// to do...

			start_accept();	
	}
}

#include "cookie.h"
//int main() {
//	printf("service begin...compile time %s %s\n", __DATE__, __TIME__);
//	g_service.initialize();
//	g_service.run();
//	printf("service end...\n");
//	return 0;
//}