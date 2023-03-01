

#ifndef __SYSTEMCALL_H
#define __SYSTEMCALL_H
#include "hardwarecommunication/interrupts.h"
#include "common/types.h"
#include "multitasking.h"
class SystemCall:public InterruptHandler{
public:
	SystemCall(InterruptManager* manager);
	~SystemCall();
	uint32_t HandleInterrupt(uint32_t esp);
};


#endif