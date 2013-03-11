#ifndef PLATFORM_H
#define PLATFORM_H
#include "platform_detail.h"

#ifdef __cplusplus
extern "C" {
#endif /*extern "C" {*/

#if (defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__))
#define  get_cpu_num get_cpu_num_win
#endif

#ifdef __cplusplus
}
#endif /*}*/

#endif /*PLATFORM_H*/