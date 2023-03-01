
#include "systemcall.h"
#include "multitasking.h"
void printf(char* str);
SystemCall::SystemCall(InterruptManager* manager):
InterruptHandler(manager,0x80+manager->HardwareInterruptOffset()){

}
SystemCall::~SystemCall(){

}
uint32_t SystemCall::HandleInterrupt(uint32_t esp){

	CPUstate* cpu = (CPUstate*)esp;
    switch(cpu->eax)
    {
        case 4:
            printf((char*)cpu->ebx);
            break;
 
        default:
            break;
    }
	return esp;
}
