

#ifndef __HARDWARECOMMUNICATION_PCI_H
#define __HARDWARECOMMUNICATION_PCI_H
/*


 */

#include "port.h"
#include "common/types.h"
#include "hardwarecommunication/interrupts.h"
#include "driver/driver.h"

enum BaseAddressRegisterType{
    MemoryMapping =0,
    InputOutput = 1
};
class BaseAddreeRegister{
public:
    bool prefetchable ; // only used in MemoryMapping register
    BaseAddressRegisterType type;
    uint32_t size;
    uint8_t* addr;
};

class PeripheralComponentInterconnectDeviceDescriptor{
public: 
    uint32_t port_base;
    uint32_t interrupt;

    uint16_t bus;
    uint16_t device;
    uint16_t function;

    uint16_t vendor_id;
    uint16_t device_id;

    uint8_t class_id;
    uint8_t subclass_id;
    uint8_t interface_id;

    uint8_t revision;


    PeripheralComponentInterconnectDeviceDescriptor();
    PeripheralComponentInterconnectDeviceDescriptor(const PeripheralComponentInterconnectDeviceDescriptor& other);
    ~PeripheralComponentInterconnectDeviceDescriptor();

};

class PeripheralComponentInterconnectController{

    Port32Bit data_port;
    Port32Bit cmd_port;

public:

    PeripheralComponentInterconnectController();
    ~PeripheralComponentInterconnectController();

    uint32_t read(uint16_t bus, uint16_t device,uint16_t function,uint32_t offset);
    void write(uint16_t bus, uint16_t device,uint16_t function,uint32_t offset,uint32_t data);
    bool deviceHasFunction(uint16_t bus, uint16_t device);

    void selectDrivers(DriverManager* driver_manager,InterruptManager* interrupt_manager);

    PeripheralComponentInterconnectDeviceDescriptor getDeviceDescriptor(uint16_t bus, uint16_t device,uint16_t function);

    BaseAddreeRegister getBaseAddressRegister(uint16_t bus, uint16_t device,uint16_t function,uint8_t num);
    Driver* getDriver(PeripheralComponentInterconnectDeviceDescriptor dev,InterruptManager* interrupt_manager);
};




#endif

