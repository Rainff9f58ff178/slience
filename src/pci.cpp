#include "hardwarecommunication/pci.h"


void printf(char* str);
void printfHex(uint8_t num);


PeripheralComponentInterconnectDeviceDescriptor::PeripheralComponentInterconnectDeviceDescriptor(){

}
PeripheralComponentInterconnectDeviceDescriptor::~PeripheralComponentInterconnectDeviceDescriptor(){

}

PeripheralComponentInterconnectController::PeripheralComponentInterconnectController()
    :data_port(0xCFC),
     cmd_port(0xCF8){


}
PeripheralComponentInterconnectDeviceDescriptor::PeripheralComponentInterconnectDeviceDescriptor(const PeripheralComponentInterconnectDeviceDescriptor& other){
    this->port_base=other.port_base;
    this->interrupt=other.interrupt;

    this->bus=other.bus;
    this->device=other.device;
    this->function=other.function;

    this->vendor_id=other.vendor_id;
    this->device_id=other.device_id;

    this->class_id=other.class_id;
    this->subclass_id=other.subclass_id;
    this->interface_id=other.interrupt;

    this->revision=other.revision;
}

PeripheralComponentInterconnectController::~PeripheralComponentInterconnectController(){

} 

/* 
    @param offset is byte nummber offset
*/
uint32_t PeripheralComponentInterconnectController::read(uint16_t bus, uint16_t device,uint16_t function,uint32_t offset){
    uint32_t id = 
        0x1<<31
        | ((bus & 0xFF) << 16)
        | ((device & 0x1F) <<11)
        | ((function & 0x07) << 8)
        | ((offset & 0xFC));

    cmd_port.Write(id);
    uint32_t result = data_port.Read();
    return result >> (8 * (offset % 4));
}


void PeripheralComponentInterconnectController::write(uint16_t bus, uint16_t device,uint16_t function,uint32_t offset,uint32_t data){
    uint32_t id = 
            0x1<<31
            | ((bus & 0xFF) << 16)
            | ((device & 0x1F) <<11)
            | ((function & 0x07) << 8)
            | ((offset & 0xFC));
    
    cmd_port.Write(id);
    data_port.Write(data);
}    

bool PeripheralComponentInterconnectController::deviceHasFunction(uint16_t bus, uint16_t device){
    return read(bus,device,0,0x0E) & (1<<7);
}

/*
    select all drivers conected to PCI to driverManager
*/
void PeripheralComponentInterconnectController::selectDrivers(DriverManager* driver_manager,InterruptManager* interrupt_manager){
    printf("{ void PeripheralComponentInterconnectController::selectDrivers\n");
    int function_num=0;
    for(int bus=0;bus<8;++bus){
        for(int device=0;device<32;++device){
            function_num=deviceHasFunction(bus,device) ? 8:1;
            for(int function=0;function<function_num;++function){
                PeripheralComponentInterconnectDeviceDescriptor dev= getDeviceDescriptor(bus,device,function);
                if(dev.vendor_id == 0 || dev.vendor_id == 0xFFFF){
                    continue;
                }
                for(int bar_num=0;bar_num<6;++bar_num){
                    BaseAddreeRegister bar = getBaseAddressRegister(bus,device,function,bar_num);
                    if(bar.addr && (bar.type == InputOutput))
                        dev.port_base = (uint32_t)bar.addr;
                }
                Driver* driver = getDriver(dev, interrupt_manager);
                if(driver != 0){
                    driver_manager->AddDriver(driver);
                }

                printf("FOUND DEVICE:");
                printf("PCI BUS:");
                printfHex(bus & 0xFF);
                printf(",DEVICE:");
                printfHex(device & 0xFF );
                printf(", FUNCTION:");
                printfHex(function & 0xFF);
                printf(" = VENDOR_ID:");
                printfHex((dev.vendor_id & 0xFF00) >> 8);
                printfHex(dev.vendor_id & 0xFF);
                printf(",DEVICE_ID:");
                printfHex((dev.device_id & 0xFF00) >> 8);
                printfHex(dev.device_id & 0xFF);
                printf("\n");

            }
        }
    }

}


PeripheralComponentInterconnectDeviceDescriptor 
PeripheralComponentInterconnectController::getDeviceDescriptor(uint16_t bus, uint16_t device,uint16_t function){
    PeripheralComponentInterconnectDeviceDescriptor result;
    result.bus=bus;
    result.device=device;
    result.function=function;


    result.vendor_id =read(bus,device,function,0x00);
    result.device_id =read(bus,device,function,0x02);
    result.class_id =read(bus,device,function,0x0b);
    result.subclass_id =read(bus,device,function,0x0a);
    result.interface_id =read(bus,device,function,0x09);
    result.revision =read(bus,device,function,0x08);
    result.interrupt =read(bus,device,function,0x3c);

    return result;
}

BaseAddreeRegister
PeripheralComponentInterconnectController::getBaseAddressRegister(uint16_t bus, uint16_t device,uint16_t function,uint8_t num){

    BaseAddreeRegister result;

    uint32_t header_type = read(bus,device,function,0x0E) &0x7F;
    int max_bars = 6-(4*header_type);
    if(num > max_bars)
        return result;

    uint32_t bar_value = read(bus,device,function,0x10+4*num);

    result.type = bar_value & 0x1 ? InputOutput : MemoryMapping;
    uint32_t temp;
    if(result.type == MemoryMapping){
        result.prefetchable = ((bar_value>>3) & 0x1) == 0x1;
        switch((bar_value>>1) & 0x3 ){
        case 0://32
        case 1://20
        case 2://64 
            //check osdev.org
        default:
            break;
        }
    }else {
        //I/O register
        result.addr = (uint8_t*)(bar_value & ~0x3);
        result.prefetchable = false;
    }
    return result;
    
}
Driver*
PeripheralComponentInterconnectController::getDriver(PeripheralComponentInterconnectDeviceDescriptor dev,
InterruptManager* interrupt_manager){
    Driver* driver=0;

    switch(dev.vendor_id){
    case 0x1022: //amd
        switch(dev.device_id){
        case 0x2000: //am79c973
            printf("detected chip am79c973\n");
          
          
            break;
        default:
            break;
        }
        break;
    case 0x8086: //intel
        break;
    default:
        break;
    }

    switch(dev.class_id){
    case 0x03: //graphic devices
        switch(dev.subclass_id){
        case 0x00: // VGA devicve
            printf("VGA device\n");
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }

    return driver; 
}








