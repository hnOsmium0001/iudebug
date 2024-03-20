#pragma once

#include "BaseDefs.h"

#include <stddef.h>

IU_EXTERN_C_BEGIN

typedef struct ScratchBuffer {
	void* buffer;
	void* lastAlloc;
} ScratchBuffer;

void ScratchBuffer_Init(ScratchBuffer*);
void ScratchBuffer_Destroy(ScratchBuffer*);

void ScratchBuffer_GrowBuffer(ScratchBuffer*);

void* ScratchBuffer_AllocNoGrow(ScratchBuffer*, size_t size, size_t alignment);
void* ScratchBuffer_Alloc(ScratchBuffer*, size_t size, size_t alignment);

/// Allocate a string of `size` on the scratch buffer, without initializing content (except setting th enull terminator).
char* AllocEmptyString(ScratchBuffer*, size_t size);
char* AllocString(ScratchBuffer*, const char* str, size_t size);

IU_EXTERN_C_END
