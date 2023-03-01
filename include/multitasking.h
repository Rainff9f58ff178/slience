

#ifndef __MULTITASKING_H
#define __MULTITASKING_H

#include "hardwarecommunication/gdt.h"
#include "common/types.h"

struct CPUstate{
//  this is what we push
	uint32_t eax;
	uint32_t ebx;
	uint32_t ecx;
	uint32_t edx;

	uint32_t esi; 
	uint32_t edi;
	uint32_t ebp;


	// uint32_t gs; 
	// uint32_t fs;
	// uint32_t es;
	// uint32_t ds;


	uint32_t error;

//  this is what sys push

	uint32_t eip;
	uint32_t cs;
	uint32_t eflags;
	uint32_t esp;
	uint32_t ss;

}__attribute__((packed));


class Task{
	friend class TaskManager;
private:
	uint8_t stack[4096]; //4 Kib
	CPUstate* cpustate;
public:
	Task(GlobalDescriptorTable* gdt,void entry_point());
	~Task();
};




class TaskManager{
	Task* tasks[256];
	int task_num;
	int current_task;
public:

	TaskManager();
	~TaskManager();
	bool addTask(Task* task);
	CPUstate* schedule_robin(CPUstate* state);
};








#endif


