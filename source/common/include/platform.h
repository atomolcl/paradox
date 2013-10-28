#ifndef PLATFORM_H
#define PLATFORM_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif /*extern "C" {*/

#if (defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__))
#define  paradox_get_cpu_num	get_cpu_num_win
#define  paradox_get_tick          get_tick_win
#define  paradox_sleep				Sleep

uint32_t get_cpu_num_win();
uint32_t get_tick_win();
#include "windows.h"
#endif /*windows*/



#ifdef __cplusplus
}
#endif /*}*/

#endif /*PLATFORM_H*/