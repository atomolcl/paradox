#include "platform_detail.h"

uint32_t get_cpu_num_win() {
	SYSTEM_INFO info;
	GetSystemInfo(&info);
	
	return info.dwNumberOfProcessors;
}