#ifndef PLAYER_H
#define PLAYER_H

#include "game_object.h"

namespace paradox {
	class connection;

	class player : public game_object {
	public:
		player() : m_connection(NULL) {}

		void set_connection(connection* connection) { m_connection = connection; }

		connection* get_connection()const { return m_connection; }

		virtual void tick(uint32_t diff) { UNUSED(diff); }

		//virtual void handle_message(const paradox_native_message& msg) {}

		void handle_net_message(const paradox_message_head* msg);

		bool send(const paradox_message_head& msg);
	private:
		connection* m_connection;
	};
}

#endif