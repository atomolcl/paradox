#ifndef PLATFORM_DETAIL_H
#define PLATFORM_DETAIL_H

#include <stdint.h>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#include "windows.h" 
#endif

#ifdef __cplusplus
extern "C" {
#endif /*extern "C" {*/

uint32_t get_cpu_num_win();

#ifdef __cplusplus
}
#endif
#endif