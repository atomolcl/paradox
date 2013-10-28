#include "ringbuffer.h"

#include <string.h>
#include "cookie.h"

namespace paradox {
#define align(x) (((x) + 3) & ~3)
	ringbuffer::ringbuffer(size_t size) {
		size = align(size);
		m_buffer_begin = new char[size];
		m_buffer_end = m_buffer_begin + size;
	}


	ringbuffer::~ringbuffer() {
		delete m_buffer_begin;
	}


	void ringbuffer::initialize() {
		m_read_point = m_write_point = m_buffer_begin;
		//to do... if or not memset
		//memset(m_buffer_begin, 0, m_buffer_end - m_buffer_begin);
	}


	void ringbuffer::write(const char* souce, size_t size) {
		if (m_write_point == m_buffer_end)
			m_write_point = m_buffer_begin;

		if (m_write_point + size > m_buffer_end) {
			size_t want_to = m_buffer_end - m_write_point;
			memcpy(m_write_point, souce, want_to);
			memcpy(m_buffer_begin, souce + want_to, size - want_to);
			m_write_point = m_buffer_begin + size - want_to;
		} else {
			memcpy(m_write_point, souce, size);
			m_write_point = m_write_point + size;
		}
	}


	bool ringbuffer::begin_read(char*& read, size_t& size) {
		if (m_read_point == m_write_point)
			return false;

		char* volatile cur_read_point = m_read_point;
		if (cur_read_point == m_buffer_end)
			cur_read_point = m_buffer_begin;

		if (cur_read_point < m_write_point) {
			paradox_assert(cur_read_point + size <= m_write_point);
		} else {
			if (cur_read_point + size > m_buffer_end)
				size = m_buffer_end - m_read_point;
		}

		read = cur_read_point;
		return true;
	}


	void ringbuffer::finish_read(size_t read_size) {
		size_t len = read_size;
		if (m_read_point < m_write_point) {
			paradox_assert(m_read_point + len <= m_write_point);
		} else {
			if (m_read_point + len > m_buffer_end) {
				len = read_size - (m_buffer_end - m_read_point);
				m_read_point = m_buffer_begin;
			}
		}

		m_read_point += len;
	}
}