

#ifndef __MEMORYMANAGER_H
#define __MEMORYMANAGER_H

#include "common/types.h"

struct MemoryChunk{

	MemoryChunk* 	prev;
	MemoryChunk* 	next;
	size_t 		 	size;
	bool			allocated;
};

class MemoryManager{
protected:
	MemoryChunk* first;
public:
	static MemoryManager* active_memory_manager;
	MemoryManager(size_t start_addr,size_t size);
	~MemoryManager();
	void* malloc(size_t size);
	void free(void* ptr);
};

void* malloc(uint32_t size);
void* operator new(uint32_t size);
void* operator new[](uint32_t size);
void* operator new(uint32_t,void* ptr);
void* operator new[](uint32_t,void* ptr);
void operator delete(void* ptr);
void operator delete[](void* ptr);
#endif