#ifndef READ_BUFFER_H
#define READ_BUFFER_H

#include "paradox_message.h"

namespace paradox { class ringbuffer; }

namespace paradox {
	class read_buffer {
	public:
		read_buffer(size_t size);
		~read_buffer();

		void initialize();

		void write(const char* source , size_t size);

		paradox_message_head* begin_read();

		void finish_read(size_t read_size);
	private:
		ringbuffer* m_ringbuffer;
		char* m_extra_buffer;
	};
}
#endif