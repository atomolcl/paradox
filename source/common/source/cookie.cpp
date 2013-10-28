#include "cookie.h"

#include <time.h>
#include <stdio.h>


uint32_t paradox::world_timer::m_last_tick = 0;

void get_time_string(char* s) {
	paradox_assert(s);
	time_t now;
	time(&now);
	tm* t = localtime(&now);
	sprintf(s, "%d/%d/%d %02d:%02d:%02d", t->tm_year + 1900,
															  t->tm_mon + 1,
															  t->tm_mday,
															  t->tm_hour,
															  t->tm_min,
															  t->tm_sec);
}