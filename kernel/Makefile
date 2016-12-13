SHELL = /bin/bash
ROOT = ..

INCLUDE = include/

M4 = m4
M4FILES = $(ROOT)/m4/global.m4 m4/kernel.m4

M4FLAGS += -I $(INCLUDE)
ASFLAGS += --32 -g
LDFLAGS += -melf_i386
CFLAGS += -c -nostdlib -nodefaultlibs -ffreestanding -no-integrated-cpp -m32 \
-Wall -Wextra -pedantic -std=c11 -xc -march=i386 #-O3
# TODO: when you can be absolutely sure a certain cpu is used (through
# configuration for example) you may replace march=i386 with some other value)

%.o: %.c
	$(M4) $(M4FILES) $(ROOT)/m4/c.m4 $< $(M4FLAGS) | \
	$(CC) $(CFLAGS) -o $@ -
%.o: %.S
	$(M4) $(M4FILES) $(ROOT)/m4/asm.m4 $< $(M4FLAGS) | \
	$(AS) -o $@ $(ASFLAGS)

%.ld.m4: %.ld
	$(M4) $(M4FILES) $< $(M4FLAGS) > $@

kernel_objs = entry.o drivers/vga.o

all: $(ROOT)/kernel.out

$(ROOT)/kernel.out: $(kernel_objs) kernel.ld.m4
	$(LD) -T kernel.ld.m4 $(kernel_objs) $(LDFLAGS) -o $@

clean:
	rm -f $(kernel_objs) *.ld.m4

.PHONY: all clean
