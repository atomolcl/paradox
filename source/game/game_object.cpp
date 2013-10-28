#include "network_service.h"
#include "game_object.h"
#include "player.h"

namespace paradox {
	logic_object::logic_object(network_service& service) : m_service(&service) {
		m_players = new player[MAX_CONNECTIONS_NUM];
	}


	logic_object::~logic_object() {
		delete[] m_players;
	}


	void logic_object::tick(uint32_t diff) {
		UNUSED(diff);

		boost::mutex::scoped_lock lock(m_mutex);
		while (!m_messages.empty()) {
			handle_message(m_messages.front());
			m_messages.pop_front();
		}
	}


	void logic_object::post(const paradox_native_message& msg) {
		boost::mutex::scoped_lock lock(m_mutex);
		m_messages.push_back(msg);
	}


	void logic_object::handle_message(const paradox_native_message& msg) {
		switch (msg.type) {
			case PNMT_PALYER_CONNECT: {
				pnmt_player_connect* m = (pnmt_player_connect*)&msg;
				m_players[m->connection_id].set_connection(m->connection);
				break;
			}

			case PNMT_PALYER_DISCONNECT: {
				pnmt_player_disconnect* m = (pnmt_player_disconnect*)&msg;
				paradox_assert(m_players[m->connection_id].get_connection());
				m_players[m->connection_id].set_connection(NULL);
				//to do 
				g_service.delete_connection(m->connection_id);
				break;
			}

			case PNMT_NETWORK: {
				pnmt_network_msg* m = (pnmt_network_msg*)&msg;
				m_players[m->connection_id].handle_net_message(m->msg);
				break;
			}

			default:
				;//to do...
		}
	}


	void logic_object::braodcast(const paradox_message_head& msg) {
		for (size_t i = 0; i < MAX_CONNECTIONS_NUM; ++i)
			m_players[i].send(msg);
	}
}