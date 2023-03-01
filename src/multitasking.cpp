
#include "multitasking.h"






Task::Task(GlobalDescriptorTable* gdt,void entry_point()){

	cpustate=(CPUstate*)(stack+4096-sizeof(CPUstate));
	cpustate->eax=0;
	cpustate->ebx=0;
	cpustate->ecx=0;
	cpustate->edx=0;

	cpustate->esi=0; 
	cpustate->edi=0;
	cpustate->ebp=0;


// There is what they were intended for, and what they are used for by Windows and Linux.
	// cpustate->gs=0; 
	// cpustate->fs=0;
	// cpustate->es=0;
	// cpustate->ds=0;

	// cpustate->error=0;

//  this is whcpustate->cs=gdt->CodeSegmentSelector();at sys push
	// cpustate->esp=0;
	cpustate->eip=(uint32_t)entry_point;
	cpustate->cs=gdt->CodeSegmentSelector();

	// cpustate->ss=0;
	cpustate->eflags=0x202;
}
Task::~Task(){

}
TaskManager::TaskManager(){
	task_num=0;
	current_task=-1;
}
TaskManager::~TaskManager(){

}
bool TaskManager::addTask(Task* task){
	if(task_num >=256)
		return false;
	tasks[task_num++]=task;
	return true;
}
CPUstate* TaskManager::schedule_robin(CPUstate* state){
	//dont switch the task
	if(task_num<=0)
		return state;
	if(current_task>=0)
		tasks[current_task]->cpustate = state;
		//store the old cpu state in task

	if(++current_task>=task_num)
		current_task %= task_num;
	//return the next task state
	return tasks[current_task]->cpustate;
}
