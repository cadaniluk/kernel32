# Parameters passed to GNU make.
# `dbg=y|n': decides whether debugging mode is on or not.
# `sep_boot=y|n': decides whether bootloader and kernel should be seperate
# images or not.
# TODO: add functionality for these parameters

SHELL = /bin/bash

%.sym: %.out
	objcopy --only-keep-debug $< $@

%.img: %.out
	cp $< tmp.out
	objcopy --strip-debug --strip-unneeded tmp.out
	objcopy -O binary tmp.out $@
	rm tmp.out

%.out:
	make -C $* all

# TODO: merge run and debug targets and decide to debug or not using the dbg
# parameter.

boot_debug: boot.sym boot.img
	qemu-system-i386 -fda boot.img -s -S &
	gdb -x dbg.gdb

all: image.img

image.img: boot.img kernel.img kernel.sym
	rm -f image.img
	bximage -mode=create -fd=1.44M -q image.img
	dd if=boot.img of=image.img bs=512 count=4 conv=notrunc
	dd if=kernel.img of=image.img bs=512 count=1 seek=4 conv=notrunc

boot.img: boot.out

kernel.sym kernel.img: kernel.out

clean:
	make -C boot clean
	make -C kernel clean
	rm -f *.out *.img *.sym bochsout.txt


.PHONY: boot_debug all clean
