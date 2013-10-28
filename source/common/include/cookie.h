#ifndef COOKIE_H
#define COOKIE_H

#include "platform.h"

#include <assert.h>
#include <stdint.h>

#if defined(PARADOX_DEBUG)
#define paradox_assert(x) assert(x)
#else
#define paradox_assert(x) ((void)(x))
#endif

#define UNUSED(x) ((void)(x))
#define MAX_CONNECTIONS_NUM 1000

void get_time_string(char* s);

namespace paradox {
	class world_timer {
	public:
		static uint32_t diff(){ uint32_t time = paradox_get_tick() - m_last_tick; m_last_tick =  paradox_get_tick(); return time; }
		static void set_last_tick(uint32_t tick) { m_last_tick = tick; }

	private:
		world_timer();
		world_timer(const world_timer&);
		const world_timer& operator= (const world_timer&);
		static uint32_t m_last_tick;
	};
}

#endif