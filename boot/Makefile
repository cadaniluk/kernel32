SHELL = /bin/bash
ROOT = ..

M4 = m4
M4DIR = $(ROOT)/m4
M4DEFFILES = $(M4DIR)/global.m4 $(M4DIR)/boot.m4

M4FLAGS += -I include/ -I $(M4DIR)
ASFLAGS += --32 -g
LDFLAGS += -melf_i386

%.o: %.S
# Prepend ".code16" first
	tmpfile=$(shell mktemp)
	echo -e ".code16\n" > $tmpfile && cat $< >> $tmpfile

	$(M4) $(M4DEFFILES) $(M4DIR)/asm.m4 $tmpfile $(M4FLAGS) | \
	$(AS) -o $@ $(ASFLAGS)
	rm $tmpfile

%.ld.m4: %.ld
	$(M4) $(M4DEFFILES) $< $(M4FLAGS) > $@


boot_objs = main.o boot.o floppy_errs.o mm_detect.o drivers/i8259A.o \
drivers/i8042.o drivers/vga.o # cmos.o apm.o

# TODO: merge run and debug targets and decide to debug or not using the dbg
# parameter.

all: $(ROOT)/boot.out

$(ROOT)/boot.out: $(boot_objs) boot.ld.m4
	$(LD) -T boot.ld.m4 $(boot_objs) $(LDFLAGS) -o $@

clean:
	rm -f $(boot_objs) *.ld.m4


.PHONY: all clean
