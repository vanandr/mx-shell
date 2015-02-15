#ifndef __DEBUG_H__
#define __DEBUG_H__
#include <stdio.h>

#ifdef DEBUG
#       define log(fmt, ...) printf(("[%s:%d] %s(): " fmt "\n"), __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__);
#else
#       define log(...)
#endif

#endif
