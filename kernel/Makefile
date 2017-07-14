SHELL = /bin/bash
ROOT = ..

M4 = m4
M4DIR = $(ROOT)/m4
M4FILES = $(M4DIR)/global.m4

ASFLAGS += --32 -g
LDFLAGS += -melf_i386
CFLAGS += -c -nostdlib -nodefaultlibs -ffreestanding -no-integrated-cpp -m32 \
-Wall -Wextra -pedantic -std=c11 -x c -march=i386 -O3
# TODO: when you can be absolutely sure a certain cpu is used (through
# configuration for example) you may replace march=i386 with some other value
CPPFLAGS += -nostdinc -P -w -I $(ROOT)/include/ -I $(M4DIR)

%.o: %.c
	$(CPP) $(CPPFLAGS) $< -D__C__ | $(M4) $(M4FILES) - | $(CC) $(CFLAGS) -o $@ -

%.o: %.S
	$(CPP) $(CPPFLAGS) $< -D__ASSEMBLER__ | $(M4) $(M4FILES) \
	$(M4DIR)/asm.m4 - | $(AS) -o $@ $(ASFLAGS)

%.ld.pp: %.ld
	$(CPP) $(CPPFLAGS) -x c $< | $(M4) $(M4FILES) - > $@

kernel_objs = entry.o drivers/vga.o descr.o events/faults.o \
events/traps.o events/aborts.o events/interrupts.o

all: $(ROOT)/kernel.out

$(ROOT)/kernel.out: $(kernel_objs) kernel.ld.pp
	$(LD) -T kernel.ld.pp $(kernel_objs) $(LDFLAGS) -o $@

clean:
	rm -f $(kernel_objs) *.ld.pp

.PHONY: all clean
