
# sudo apt-get install g++ binutils libc6-dev-i386
# sudo apt-get install VirtualBox grub-legacy xorriso


GCCPARAMS = -m32 -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions \
-fno-leading-underscore -Wno-write-strings -fpermissive -fno-stack-protector -Iinclude \
-fpermissive

ASPARAMS = --32
LDPARAMS = -melf_i386

objects = obj/loader.o \
	obj/gdt.o  \
	obj/driver.o \
	obj/port.o \
	obj/interruptstubs.o \
	obj/interrupts.o \
	obj/keyboard.o \
	obj/mouse.o \
	obj/kernel.o \
	obj/pci.o \
	obj/vga.o \
	obj/gui/windows.o \
	obj/gui/widget.o \
	obj/gui/desktop.o \
	obj/gui/color.o \
	obj/multitasking.o \
	obj/memorymanager.o \
	obj/systemcall.o



obj/%.o: src/%.cpp
	mkdir -p $(@D)
	gcc $(GCCPARAMS) -c -o $@ $<

obj/%.o: src/%.s
	mkdir -p $(@D)
	as $(ASPARAMS) -o $@ $<

mykernel.bin: linker.ld $(objects)
	ld $(LDPARAMS) -T $< -o $@ $(objects)

mykernel.iso: mykernel.bin
	mkdir iso
	mkdir iso/boot
	mkdir iso/boot/grub
	cp mykernel.bin iso/boot/mykernel.bin
	echo 'set timeout=0'                      > iso/boot/grub/grub.cfg
	echo 'set default=0'                     >> iso/boot/grub/grub.cfg
	echo ''                                  >> iso/boot/grub/grub.cfg
	echo 'menuentry "My Operating System" {' >> iso/boot/grub/grub.cfg
	echo '  multiboot /boot/mykernel.bin'    >> iso/boot/grub/grub.cfg
	echo '  boot'                            >> iso/boot/grub/grub.cfg
	echo '}'                                 >> iso/boot/grub/grub.cfg
	grub-mkrescue --output=mykernel.iso iso
	rm -rf iso

install: mykernel.bin
	sudo cp $< /boot/mykernel.bin


run: mykernel.iso
	(killall /usr/lib/virtualbox/VirtualBox && sleep 1) || true
	VirtualBoxVM --startvm 'mykernelback'     
.PHONY: clean
clean:
	rm -f $(objects) mykernel.bin mykernel.iso
