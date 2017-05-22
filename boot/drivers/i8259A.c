#include <i8259A.h>

void i8259A_wait(void)
{
	__asm__(
	"jmp 1f\n\t"
	"1:\n\t"
	"jmp 1f\n\t"
	"1:"
	);
}