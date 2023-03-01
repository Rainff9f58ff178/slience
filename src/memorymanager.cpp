

#include "memorymanager.h"





MemoryManager* MemoryManager::active_memory_manager=0;

MemoryManager::MemoryManager(size_t start_addr,size_t size){

	active_memory_manager=this;
	if(size < sizeof(MemoryChunk)){
		first=0;
	}else{
		first=(MemoryChunk*)start_addr;
		first->size = size - sizeof(MemoryChunk);
		first->prev=0;
		first->next=0;
		first->allocated=false;
	}

}
MemoryManager::~MemoryManager(){
	if(active_memory_manager==this)
		active_memory_manager=0;
}
void* MemoryManager::malloc(size_t size){
	MemoryChunk* result=0;
	for(MemoryChunk* chunk=first;chunk!=0 && result==0;chunk=chunk->next){
		if(chunk->size >size && chunk->allocated==false){
			result=chunk;
		}
	}

	if(result==0)
		return 0;
	if(result->size >= size + sizeof(MemoryChunk)+1){
		
		//result->size >= size + sizeof(MemoryChunk)+1
		 MemoryChunk* temp = (size_t)result+sizeof(MemoryChunk)+size;
		 temp->prev=result;
		 temp->size=result->size - size -sizeof(MemoryChunk);
		 temp->next=result->next;

		 if(temp->next!=0)
		 	temp->next->prev=temp;
		 result->size=size;
		 result->next=temp;
	}

	result->allocated=true;
	return (void*)((size_t)result+sizeof(MemoryChunk));
}
void MemoryManager::free(void* ptr){
	MemoryChunk* chunk_ptr_belong=(MemoryChunk*)((size_t)ptr-sizeof(MemoryChunk));

	chunk_ptr_belong->allocated=false;

	MemoryChunk* next_chunk=chunk_ptr_belong->next;
	MemoryChunk* before_chunk=chunk_ptr_belong->prev;
	if(next_chunk!=0 && next_chunk->allocated==false){
		chunk_ptr_belong->next=next_chunk->next;
		if(chunk_ptr_belong->next!=0)
			chunk_ptr_belong->next->prev=chunk_ptr_belong;
		chunk_ptr_belong->size+=sizeof(MemoryChunk)+next_chunk->size;
	}
	if(before_chunk!=0 && before_chunk->allocated==false){
		before_chunk->size +=sizeof(MemoryChunk)+chunk_ptr_belong->size;
		before_chunk->next=chunk_ptr_belong->next;
		if(before_chunk->next!=0){
			before_chunk->next->prev=before_chunk;
		}
	}

}



void* operator new(uint32_t size){
	if(MemoryManager::active_memory_manager==0)
		return 0;
	return MemoryManager::active_memory_manager->malloc(size);
}
void* malloc(uint32_t size){
	if(MemoryManager::active_memory_manager==0)
		return 0;
	return MemoryManager::active_memory_manager->malloc(size);	
}
void* operator new[](uint32_t size){
	if(MemoryManager::active_memory_manager==0)
		return 0;
	return MemoryManager::active_memory_manager->malloc(size);
}


void operator delete(void* ptr){
	if(MemoryManager::active_memory_manager!=0)
		MemoryManager::active_memory_manager->free(ptr);
}
void operator delete[](void* ptr){
	if(MemoryManager::active_memory_manager!=0)
		MemoryManager::active_memory_manager->free(ptr);
}
void* operator new(uint32_t,void* ptr){
	return ptr;
}
void* operator new[](uint32_t,void* ptr){
	return ptr;
}