#ifndef PARADOX_MESSAGE_H
#define PARADOX_MESSAGE_H

#include <stdint.h>
#include <string.h>

#pragma pack(push, 4)


/*server to client*/
enum paradox_network_message_type {
	PNWMT_INVALID = 0,
	PNWMT_TEST = 1
};

#define  PARADOX_MESSAGE_SHIFT 16

enum paradox_native_message_type {
	PNMT_INVALID = 0,
	PNMT_NETWORK = 1,
	PNMT_PALYER_CONNECT = 2,
	PNMT_PALYER_DISCONNECT = 3
};
namespace paradox {
	struct paradox_message_head {
		uint16_t size;
		uint16_t type;

		paradox_message_head() : size (0), type(PNWMT_INVALID) {}
	};

	#define PRARADOX_MESSAGE_HEAD_SIZE (sizeof(((paradox_message_head*)0)->size))


	struct test_message : public paradox_message_head {
		char chat[37];

		test_message() {
			size = sizeof(*this);
			type = PNWMT_TEST;
			memset(chat , 0, sizeof(chat));
		}
	};

	/*************************************************************/
	struct paradox_native_message {
		uint32_t type;

		paradox_native_message() {
			type = 0;
		}
	};

	/*PNMT_PALYER_CONNECT*/
	namespace paradox { class connection; }
	struct pnmt_player_connect : public paradox_native_message {
		int16_t connection_id;
		connection* connection;

		pnmt_player_connect() : connection_id(-1) {
			type = PNMT_PALYER_CONNECT;
		}
	};

	/*PNMT_PALYER_DISCONNECT*/
	struct pnmt_player_disconnect : public paradox_native_message {
		int16_t connection_id;

		pnmt_player_disconnect() : connection_id(-1) {
			type = PNMT_PALYER_DISCONNECT;
		}
	};

	struct pnmt_network_msg : public paradox_native_message {
		int16_t connection_id;
		paradox_message_head* msg;

		pnmt_network_msg() : connection_id(-1), msg(NULL) {
			type = PNMT_NETWORK;
		}
	};
}
#pragma pack(pop)
#endif