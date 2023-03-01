
#include "common/types.h"
#include "hardwarecommunication/gdt.h"
#include "hardwarecommunication/interrupts.h"
#include "driver/driver.h"
#include "driver/keyboard.h"
#include "driver/mouse.h"
#include "hardwarecommunication/pci.h"
#include "driver/vga.h"
#include "gui/desktop.h"
#include "gui/color.h"
#include "gui/windows.h"
#include "multitasking.h"
#include "memorymanager.h"
#include "systemcall.h"
// #define GRAPHIC_MODE

#define MULTI_TASK
void printf(char* str)
{
    static uint16_t* VideoMemory = (uint16_t*)0xb8000;

    static uint8_t x=0,y=0;

    for(int i = 0; str[i] != '\0'; ++i)
    {
        switch(str[i])
        {
            case '\n':
                x = 0;
                y++;
                break;
            default:
                VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0xFF00) | str[i];
                x++;
                break;
        }

        if(x >= 80)
        {
            x = 0;
            y++;
        }

        if(y >= 25)
        {
            for(y = 0; y < 25; y++)
                for(x = 0; x < 80; x++)
                    VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0xFF00) | ' ';
            x = 0;
            y = 0;
        }
    }
}

void printfHex(uint8_t key)
{
    char* foo = "00";
    char* hex = "0123456789ABCDEF";
    foo[0] = hex[(key >> 4) & 0xF];
    foo[1] = hex[key & 0xF];
    printf(foo);
}
void print32BitHex(uint32_t key){
    printfHex((key>>24)&0xFF);
    printfHex((key>>16)&0xFF);
    printfHex((key>>8)&0xFF);
    printfHex((key)&0xFF);
}



class PrintfKeyboardEventHandler : public KeyboardEventHandler
{
public:
    void OnKeyDown(char c)
    {
        char* foo = " ";
        foo[0] = c;
        printf(foo);
    }
};

class MouseToConsole : public MouseEventHandler
{
    int8_t x, y;
public:
    
    MouseToConsole()
    {
    }
    
    virtual void OnActivate()
    {
        uint16_t* VideoMemory = (uint16_t*)0xb8000;
        x = 40;
        y = 12;
        VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0x0F00) << 4
                            | (VideoMemory[80*y+x] & 0xF000) >> 4
                            | (VideoMemory[80*y+x] & 0x00FF);        
    }
    
    virtual void OnMouseMove(int xoffset, int yoffset)
    {
        static uint16_t* VideoMemory = (uint16_t*)0xb8000;
        VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0x0F00) << 4
                            | (VideoMemory[80*y+x] & 0xF000) >> 4
                            | (VideoMemory[80*y+x] & 0x00FF);

        x += xoffset;
        if(x >= 80) x = 79;
        if(x < 0) x = 0;
        y += yoffset;
        if(y >= 25) y = 24;
        if(y < 0) y = 0;

        VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0x0F00) << 4
                            | (VideoMemory[80*y+x] & 0xF000) >> 4
                            | (VideoMemory[80*y+x] & 0x00FF);
    }
    
};







typedef void (*constructor)();
extern "C" constructor start_ctors;
extern "C" constructor end_ctors;
extern "C" void callConstructors()
{
    for(constructor* i = &start_ctors; i != &end_ctors; i++)
        (*i)();
}

void sysprintf(char* str)
{
    asm("int $0x80" : : "a" (4), "b" (str));
}
void taskA(){
    while(true)
        sysprintf("A");
}
void taskB(){
    while(true)
        sysprintf("B");
}

extern "C" void kernelMain(const void* multiboot_structure, uint32_t /*multiboot_magic*/)
{

    //initialize dynamic memory magment:
    size_t heap=10*1024*1024; //in ten Mib addr
    uint32_t* memupper=(uint32_t*)(multiboot_structure+8);
    MemoryManager memory_manager(heap,(*memupper)*1024-heap-10*1024);

    printf("\nHello World! --- TIME 2\n");

    GlobalDescriptorTable gdt;

    printf("heap Initialize in :0x");
    print32BitHex(heap);


#ifdef MULTI_TASK
    TaskManager task_manager;
    Task task1(&gdt,taskA);
    Task task2(&gdt,taskB);
    task_manager.addTask(&task1);
    task_manager.addTask(&task2);
    InterruptManager interrupts(0x20, &gdt,&task_manager);
#else
    InterruptManager interrupts(0x20, &gdt,0);
#endif
    printf("Initializing Hardware, Stage 1\n");
    
    DriverManager drvManager;
#ifdef GRAPHIC_MODE
    Desktop desktop(320,200,0x00,0x00,0xA8);
    KeyboardDriver keyboard(&interrupts, &desktop);
    MouseDriver mouse(&interrupts, &desktop);
#else
    PrintfKeyboardEventHandler printf_handler;
    KeyboardDriver keyboard(&interrupts, &printf_handler);
    MouseToConsole mouse_handler;
    MouseDriver mouse(&interrupts,&mouse_handler);
#endif
    drvManager.AddDriver(&keyboard);
    drvManager.AddDriver(&mouse);
    SystemCall system_call(&interrupts);

#ifdef GRAPHIC_MODE
    VideoGraphicsArray vga;
#endif
    PeripheralComponentInterconnectController pciController;
    pciController.selectDrivers(&drvManager,&interrupts);
    printf("Initializing Hardware, Stage 2\n");
    drvManager.ActivateAll();
    
    printf("Initializing Hardware, Stage 3\n");
#ifdef GRAPHIC_MODE
    vga.SetMode(320,200,8);
    Windows win1(&desktop,40,15,90,90,0x00,0xA8,0x00);
    Windows win2(&win1,0,0,20,20,0xA8,0x00,0x00);
    win1.addChild(&win2);
    desktop.addChild(&win1);
#endif
    interrupts.Activate();

    while(1){
        #ifdef GRAPHIC_MODE
        desktop.Draw(&vga);
        #endif
    }
}
