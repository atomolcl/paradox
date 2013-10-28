#include "read_buffer.h"
#include "ringbuffer.h"

#include <string.h>
#include <cookie.h>

namespace paradox {
	static const size_t EXTRA_BUFFER_SIZE = 512;

	read_buffer::read_buffer(size_t size) {
		m_ringbuffer = new ringbuffer(size);
		//to do ...
		m_extra_buffer = new char[EXTRA_BUFFER_SIZE];
	}


	read_buffer::~read_buffer() {
		delete m_ringbuffer;
		delete[] m_extra_buffer;
	}


	void read_buffer::initialize() {
		m_ringbuffer->initialize();
		//memset(m_extra_buffer, 0, EXTRA_BUFFER_SIZE);
	}


	void read_buffer::write(const char* source , size_t size) {
		/*just paradox_assert for test*/
		char * volatile read_point = m_ringbuffer->get_read_point();
		char * volatile write_point = m_ringbuffer->get_write_point();
		bool read_bigger = read_point > write_point;
		bool condition = write_point + size > m_ringbuffer->get_buffer_end();

		m_ringbuffer->write(source, size);

		/*just paradox_assert for test*/
		read_point = m_ringbuffer->get_read_point();
		write_point = m_ringbuffer->get_write_point();
		paradox_assert(!(read_bigger && write_point > read_point));
		paradox_assert(!(!read_bigger && write_point > read_point && condition));
	}


	paradox_message_head* read_buffer::begin_read() {
		size_t size_head = sizeof(((paradox_message_head*)0)->size);
		size_t size = size_head;
		char* read;
		paradox_assert(m_ringbuffer->begin_read(read, size));

		size_t all_size;
		if (size == size_head)
			all_size = (size_t)(*(uint16_t*)read);
		else {
			char temp[sizeof(((paradox_message_head*)0)->size)] = {0};
			size = 1; /*read one byte*/
			paradox_assert(m_ringbuffer->begin_read(read, size));
			paradox_assert(size == 1);
			memcpy(temp, read, size);
			memcpy(temp + size, m_ringbuffer->get_buffer_begin(), PRARADOX_MESSAGE_HEAD_SIZE - size);
			all_size = (size_t)(*(uint16_t*)temp);
		}
		
		size_t want_to = all_size;
		paradox_assert(m_ringbuffer->begin_read(read, want_to));
		if (all_size == want_to)
			return (paradox_message_head*)read;
		else {
			memcpy(m_extra_buffer, read, want_to);
			memcpy(m_extra_buffer + want_to, m_ringbuffer->get_buffer_begin(), all_size - want_to);
			paradox_assert(m_ringbuffer->get_buffer_begin() + all_size - want_to <= m_ringbuffer->get_write_point());
			return (paradox_message_head*)m_extra_buffer;
		}
	}


	void read_buffer::finish_read(size_t read_size) {
		m_ringbuffer->finish_read(read_size);
	}
}