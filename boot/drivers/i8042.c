#include <i8042.h>

#if 0 == 1
/* Sets the content of the command byte. */
#define i8042_SET_CMD_BYTE dnl
	push %ax

	mov %al, %ah
	mov $i8042_CMD_WR_CMD, %al
	i8042_WR_CMD

	xchg %ah, %al
	i8042_WR_DATA

	pop %ax


/* Returns the command byte. */
#define i8042_CMD_BYTE dnl
	mov $i8042_CMD_RD_CMD, %al
	i8042_WR_CMD
	i8042_RD_DATA



/* Reads from the input port. */ 
#define i8042_RD_INP dnl
	mov $i8042_CMD_RD_INP, %al
	i8042_WR_CMD
	i8042_RD_DATA


/* Reads from the output port. */
#define i8042_RD_OUTP dnl
	mov $i8042_CMD_RD_OUTP, %al
	i8042_WR_CMD
	i8042_RD_DATA


/* Writes to the output port. */
#define i8042_WR_OUTP dnl
	push %ax

	mov %al, %ah
	mov $i8042_CMD_WR_OUTP, %al
	i8042_WR_CMD

	xchg %ah, %al
	i8042_WR_DATA

	pop %ax


/* Wait until the input buffer is empty. */
#define i8042_WIN_EMPTY dnl
	push %ax
1:
	in $i8042_STATUS, %al
	test $i8042_STAT_IN/* Sets the content of the command byte. */
#define i8042_SET_CMD_BYTE dnl
	push %ax

	mov %al, %ah
	mov $i8042_CMD_WR_CMD, %al
	i8042_WR_CMD

	xchg %ah, %al
	i8042_WR_DATA

	pop %ax


/* Returns the command byte. */
#define i8042_CMD_BYTE dnl
	mov $i8042_CMD_RD_CMD, %al
	i8042_WR_CMD
	i8042_RD_DATA



/* Reads from the input port. */ 
#define i8042_RD_INP dnl
	mov $i8042_CMD_RD_INP, %al
	i8042_WR_CMD
	i8042_RD_DATA


/* Reads from the output port. */
#define i8042_RD_OUTP dnl
	mov $i8042_CMD_RD_OUTP, %al
	i8042_WR_CMD
	i8042_RD_DATA


/* Writes to the output port. */
#define i8042_WR_OUTP dnl
	push %ax

	mov %al, %ah
	mov $i8042_CMD_WR_OUTP, %al
	i8042_WR_CMD

	xchg %ah, %al
	i8042_WR_DATA

	pop %ax


/* Wait until the input buffer is empty. */
#define i8042_WIN_EMPTY dnl
	push %ax
1:
	in $i8042_STATUS, %al
	test $i8042_STAT_INEMPTY, %al
	jnz 1f
	pause
	jmp 1b
1:
	pop %ax


/* Wait until the output buffer is full. */
#define i8042_WOUT_FULL dnl
	push %ax
1:
	in $i8042_STATUS, %al
	test $i8042_STAT_OUTFULL, %al
	jnz 1f
	pause
	jmp 1b
1:
	pop %ax

 
/* Write to the data register. */
#define i8042_WR_DATA dnl
	i8042_WIN_EMPTY
	out %al, $i8042_DATA


/* Write to the command register. */
#define i8042_WR_CMD dnl
	i8042_WIN_EMPTY
	out %al, $i8042_CMD


/* Read from the data register. */
#define i8042_RD_DATA dnl
	i8042_WOUT_FULL
	in $i8042_DATA, %alEMPTY, %al
	jnz 1f
	pause
	jmp 1b
1:
	pop %ax


/* Wait until the output buffer is full. */
#define i8042_WOUT_FULL dnl
	push %ax
1:
	in $i8042_STATUS, %al
	test $i8042_STAT_OUTFULL, %al
	jnz 1f
	pause
	jmp 1b
1:
	pop %ax

 
/* Write to the data register. */
#define i8042_WR_DATA dnl
	i8042_WIN_EMPTY
	out %al, $i8042_DATA


/* Write to the command register. */
#define i8042_WR_CMD dnl
	i8042_WIN_EMPTY
	out %al, $i8042_CMD


/* Read from the data register. */
#define i8042_RD_DATA dnl
	i8042_WOUT_FULL
	in $i8042_DATA, %al

#endif