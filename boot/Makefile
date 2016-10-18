# Parameters passed to GNU make.
# `dbg=y|n': decides whether debugging mode is on or not.
# TODO: implement these parameters

SHELL = /bin/bash
ROOT = ..

M4 = m4
M4PATH = "include/:$(ROOT)/"
M4GLOBAL = $(ROOT)/m4/global.m4

ASFLAGS += --32 -g
LDFLAGS += -melf_i386

# TODO: maybe add a variable for m4/asm.m4 and m4/c.m4 like m4/global.m4

%.o: %.S
	$(M4) $(M4GLOBAL) $(ROOT)/m4/asm.m4 $< $(M4FLAGS) | \
	$(AS) -o $@ $(ASFLAGS)

%.ld.m4: %.ld
	$(M4) $(M4GLOBAL) $< $(M4FLAGS) > $@


boot_objs = boot.o main.o floppy_errs.o

# TODO: merge run and debug targets and decide to debug or not using the dbg
# parameter.

all: $(ROOT)/boot.out

$(ROOT)/boot.out: $(boot_objs) boot.ld.m4
	$(LD) -T boot.ld.m4 $(boot_objs) $(LDFLAGS)

clean:
	rm -f $(boot_objs) *.out *.ld.m4


.PHONY: all clean run
