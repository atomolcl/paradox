#include "platform.h"

uint32_t get_cpu_num_win() {
	SYSTEM_INFO info;
	GetSystemInfo(&info);
	
	return info.dwNumberOfProcessors;
}

uint32_t get_tick_win() {
	return (uint32_t)GetTickCount();
}