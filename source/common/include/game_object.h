#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "paradox_message.h"
#include "cookie.h"

#include <list>
#include <boost/thread/mutex.hpp>

namespace paradox { class network_service; }

namespace paradox {
	class game_object {
	public:
		virtual void initialize() {}

		virtual void finalize() {}

		virtual void tick(uint32_t) {}

		virtual void post(const paradox_native_message&) {}

		virtual void handle_message(const paradox_native_message&) {}

		virtual ~game_object() {}
	};


	class player;
	class logic_object : public game_object {
	public:
		logic_object(network_service& service);
		virtual ~logic_object();

		virtual void tick(uint32_t diff);

		virtual void post(const paradox_native_message& msg);

		virtual void handle_message(const paradox_native_message& msg);

		void braodcast(const paradox_message_head& msg);
	private:
		//to do ...memory pool
		std::list<paradox_native_message> m_messages;
		network_service* m_service;
		player* m_players;
		boost::mutex m_mutex;
	};
}
#endif