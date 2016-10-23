# Parameters passed to GNU make.
# `dbg=y|n': decides whether debugging mode is on or not.
# `sep_boot=y|n': decides whether bootloader and kernel should be seperate
# images or not.
# TODO: add functionality for these parameters

SHELL = /bin/bash

%.sym: %.out
	objcopy --strip-debug --strip-unneeded $<
	objcopy --only-keep-debug $< $@

%.img: %.out
	objcopy -O binary $< $@

%.out:
	make -C $* all

# TODO: merge run and debug targets and decide to debug or not using the dbg
# parameter.

boot_debug: boot.sym boot.img
	qemu-system-i386 -fda boot.img -s -S &
	gdb -x dbg.gdb

all: boot.img #kernel.img kernel.sym

boot.img: boot.out

kernel.sym kernel.img: kernel.out


clean:
	make -C boot clean
	make -C kernel clean
	rm -f *.out *.img *.sym bochsout.txt


.PHONY: boot_debug all clean
