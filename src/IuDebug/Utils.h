#pragma once

#include "BaseDefs.h"

#include <stddef.h>

IU_EXTERN_C_BEGIN

inline bool IsPowerOfTwo(size_t v) {
	return (v & (v - 1)) == 0;
}

inline size_t NextPowerOfTwo(size_t v) {
	v--;
	v |= v >> 1;
	v |= v >> 2;
	v |= v >> 4;
	v |= v >> 8;
	v |= v >> 16;
	v++;
	return v;
}

IU_EXTERN_C_END
