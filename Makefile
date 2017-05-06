# Parameters passed to GNU make.
# `dbg=y|n': decides whether debugging mode is on or not.
# `sep_boot=y|n': decides whether bootloader and kernel should be seperate
# images or not.
# TODO: add functionality for these parameters

SHELL = /bin/bash

# TODO: not needed by bochs (i just use objdump to obtain addresses), but
# maybe a symbol file will be useful later. just keep it commented out.
#%.sym: %.out
#	objcopy --only-keep-debug $< $@

%.img: %.out
	cp $< tmp.out
	objcopy --strip-debug --strip-unneeded tmp.out
	objcopy -O binary tmp.out $@
	rm tmp.out

%.out:
	make -C $* all

# TODO: merge run and debug targets and decide to debug or not using the dbg
# parameter.

all: image.img

image.img: boot.img kernel.img
	rm -f image.img
	bximage -mode=create -fd=1.44M -q image.img
# TODO: replace count with BOOTSYS_SECTORS+1 (for prim. bootloader)!!!!!!
	dd if=boot.img of=image.img bs=512 count=4 conv=notrunc
# TODO: replace count with KERNEL_SECTORS!!!!!
	dd if=kernel.img of=image.img bs=512 count=8 seek=4 conv=notrunc

boot.img: boot.out

kernel.img: kernel.out

clean:
	make -C boot clean
	make -C kernel clean
	rm -f *.out *.img *.sym bochsout.txt


.PHONY: all clean
