SHELL = /bin/bash
ROOT = ..

M4 = m4
M4DIR = $(ROOT)/m4
M4FILES = $(M4DIR)/global.m4

ASFLAGS += --32 -g
LDFLAGS += -melf_i386

CPPFLAGS += -P -w -I $(ROOT)/include/ -I $(M4DIR)

%.o: %.S
	$(CPP) $< $(CPPFLAGS) | $(M4) $(M4DEFFILES) $(M4DIR)/asm.m4 \
	$(M4FLAGS) - | $(AS) -o $@ $(ASFLAGS)

%.ld.pp: %.ld
	$(CPP) $(CPPFLAGS) -x c $< | $(M4) $(M4FILES) - > $@

boot_objs = main.o boot.o floppy_errs.o mm_detect.o drivers/vga.o
# cmos.o apm.o drivers/i8042.o

# TODO: merge run and debug targets and decide to debug or not using the dbg
# parameter.

all: $(ROOT)/boot.out

$(ROOT)/boot.out: $(boot_objs) boot.ld.pp
	$(LD) -T boot.ld.pp $(boot_objs) $(LDFLAGS) -o $@

clean:
	rm -f $(boot_objs) *.ld.pp


.PHONY: all clean
