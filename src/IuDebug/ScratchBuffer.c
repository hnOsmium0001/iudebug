#include "ScratchBuffer.h"

#include "Utils.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define SCRATCH_BUFFER_BLOCK_SIZE 32768 // 32*1024

typedef struct {
	void* prev;
	//size_t size;
} ScratchBlockHeader;

void ScratchBuffer_Init(ScratchBuffer* scratch) {
	scratch->buffer = malloc(SCRATCH_BUFFER_BLOCK_SIZE);
	scratch->lastAlloc = scratch->buffer + SCRATCH_BUFFER_BLOCK_SIZE;

	ScratchBlockHeader* sbh = (ScratchBlockHeader*)scratch->buffer;
	sbh->prev = NULL;
	//sbh->size = SCRATCH_BUFFER_BLOCK_SIZE;
}

void ScratchBuffer_Destroy(ScratchBuffer* scratch) {
	void* buf = scratch->buffer;
	while (buf != NULL) {
		void* prev = ((ScratchBlockHeader*)buf)->prev;
		free(buf);
		buf = prev;
	}
}

void ScratchBuffer_GrowBuffer(ScratchBuffer* scratch) {
	void* newBuffer = malloc(SCRATCH_BUFFER_BLOCK_SIZE);
	ScratchBlockHeader* sbh = (ScratchBlockHeader*)newBuffer;
	sbh->prev = scratch->buffer;
	//sbh->size = SCRATCH_BUFFER_BLOCK_SIZE;

	scratch->buffer = newBuffer;
	scratch->lastAlloc = newBuffer + SCRATCH_BUFFER_BLOCK_SIZE;
}

static uintptr_t BumpDownAndAlign(uintptr_t start, size_t size, size_t alignment) {
	size_t unaligned = start - size;
	return unaligned & ~(alignment - 1);
}

void* ScratchBuffer_AllocNoGrow(ScratchBuffer* scratch, size_t size, size_t alignment) {
	assert(IsPowerOfTwo(alignment));

	uintptr_t addr = BumpDownAndAlign((uintptr_t)scratch->lastAlloc, size, alignment);
	uintptr_t bufferBeg = (uintptr_t)scratch->buffer + sizeof(ScratchBlockHeader);
	if (addr < bufferBeg)
		return NULL;

	scratch->lastAlloc = (void*)addr;
	return (void*)addr;
}

void* ScratchBuffer_Alloc(ScratchBuffer* scratch, size_t size, size_t alignment) {
	void* res = ScratchBuffer_AllocNoGrow(scratch, size, alignment);
	if (res != NULL)
		return res;

	ScratchBuffer_GrowBuffer(scratch);
	void* res1 = ScratchBuffer_AllocNoGrow(scratch, size, alignment);
	assert(res1 != NULL, "Alloc too big for scratch buffer");
	return res1;
}

char* AllocEmptyString(ScratchBuffer* scratch, size_t size) {
	char* res = ScratchBuffer_Alloc(scratch, size + 1);
	res[size] = '\0';
	return res;
}

char* AllocString(ScratchBuffer*, const char* str, size_t size) {
	char* res = ScratchBuffer_Alloc(scratch, size + 1);
	strncpy(res, str, size);
	return res;
}
