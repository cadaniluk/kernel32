# Parameters passed to GNU make.
# `dbg=y|n': decides whether debugging mode is on or not.
# `sep_boot=y|n': decides whether bootloader and kernel should be seperate
# images or not.
# TODO: add functionality for these parameters

SHELL = /bin/bash

M4 = m4
M4FLAGS = 
M4GLOBAL = global.m4

ASFLAGS += --32 -g
CFLAGS += -m32 -std=c11 -Wall -Wextra -pedantic -g # -O3
LDFLAGS += -melf_i386

%.o: %.S
	$(M4) $(M4GLOBAL) $< $(M4FLAGS) | $(AS) -o $@ $(ASFLAGS)

%.o: %.c
	$(M4) $(M4GLOBAL) $< $(M4FLAGS) | $(CC) -c -o $@ $(CFLAGS)

%.sym: %.out
	objcopy --only-keep-debug $< $@

%.img: %.out
	objcopy --strip-debug --strip-unneeded $<
	objcopy -O binary $< $@


boot_objs = boot/main.o
bootsys_objs = bootsys/main.o
kernel_objs = 

# TODO: merge run and debug targets and decide to debug or not using the dbg
# parameter.

boot_debug: boot.sym boot.img
	qemu-system-i386 -fda boot.img -s -S &
	gdb -x boot.gdb

bootsys_debug: bootsys.sym bootsys.img

all: boot.sym boot.img bootsys.sym bootsys.img #kernel.img kernel.sym

boot.img: boot.out
boot.sym: boot.out
boot.out: $(boot_objs)
	$(LD) -T boot.ld $(boot_objs) $(LDFLAGS)

bootsys.img: bootsys.out
bootsys.sym: bootsys.out
bootsys.out: $(bootsys_objs)
	$(LD) -T bootsys.ld $(bootsys_objs) $(LDFLAGS)

kernel.sym: kernel.out
kernel.img: kernel.out
kernel.out: $(kernel_objs)
	$(LD) -T kernel.ld $(kernel_objs) $(LDFLAGS)

clean:
	rm -f $(boot_objs) $(bootsys_objs) $(kernel_objs) *.out *.img *.sym


.PHONY: boot_debug all clean run
