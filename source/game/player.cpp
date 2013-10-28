#include "connection.h"
#include "player.h"

namespace paradox {
	void player::handle_net_message(const paradox_message_head* msg) {
		switch (msg->type) {

		}

		if (m_connection)
			m_connection->finish_read(msg->size);
	}


	bool player::send(const paradox_message_head& msg) {
		bool ret = false;
		if (m_connection)
			ret = m_connection->send(&msg, msg.size) > 0;
		
		return ret;
	}
}