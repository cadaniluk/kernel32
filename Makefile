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
CFLAGS += -std=c11 -Wall -Wextra -pedantic -g # -O3
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
kernel_objs = 

# TODO: merge run and debug targets and decide to debug or not using the dbg
# parameter.

run: boot.img # kernel.img
	qemu-system-i386 -fda $<

debug: boot.img #kernel.img
	qemu-system-i386 -fda $< -s -S

all: boot.img boot.sym #kernel.img kernel.sym

boot.img: boot.out
boot.sym: boot.out
boot.out: $(boot_objs)
	$(LD) -T boot.ld $(boot_objs) $(LDFLAGS)

kernel.img: kernel.out
kernel.sym: kernel.out
kernel.out: $(kernel_objs)
	$(LD) -T kernel.ld $(kernel_objs) $(LDFLAGS)

clean:
	rm -f $(boot_objs) $(kernel_objs) *.out *.img *.sym


.PHONY: all clean run
