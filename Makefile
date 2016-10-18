# Parameters passed to GNU make.
# `dbg=y|n': decides whether debugging mode is on or not.
# `sep_boot=y|n': decides whether bootloader and kernel should be seperate
# images or not.
# TODO: add functionality for these parameters

SHELL = /bin/bash

M4 = m4
M4FLAGS = 
M4GLOBAL = m4/global.m4

ASFLAGS += --32 -g
CFLAGS += -m32 -std=c11 -Wall -Wextra -pedantic -g # -O3
LDFLAGS += -melf_i386

# TODO: maybe add a variable for m4/asm.m4 and m4/c.m4 like m4/global.m4

%.o: %.S
	$(M4) $(M4GLOBAL) m4/asm.m4 $< $(M4FLAGS) | $(AS) -o $@ $(ASFLAGS)

%.o: %.c
	$(M4) $(M4GLOBAL) m4/c.m4 $< $(M4FLAGS) | $(CC) -c -o $@ $(CFLAGS)

%.sym: %.out
	objcopy --only-keep-debug $< $@

%.img: %.out
	objcopy --strip-debug --strip-unneeded $<
	objcopy -O binary $< $@

%.ld.m4: %.ld
	$(M4) $(M4GLOBAL) $< $(M4FLAGS) > $@


boot_objs = boot/boot.o boot/main.o boot/floppy_errs.o
kernel_objs = 

# TODO: merge run and debug targets and decide to debug or not using the dbg
# parameter.

boot_debug: boot.sym boot.img
	qemu-system-i386 -fda boot.img -s -S &
	gdb -x boot.gdb

all: boot.sym boot.img #kernel.img kernel.sym

boot.img: boot.out
boot.sym: boot.out
boot.out: $(boot_objs) boot.ld.m4
	$(LD) -T boot.ld.m4 $(boot_objs) $(LDFLAGS)

kernel.sym: kernel.out
kernel.img: kernel.out
kernel.out: $(kernel_objs) kernel.ld.m4
	$(LD) -T kernel.ld.m4 $(kernel_objs) $(LDFLAGS)

clean:
	rm -f $(boot_objs) $(boot_objs) $(kernel_objs) *.out *.img *.sym \
	*.ld.m4


.PHONY: boot_debug all clean run
