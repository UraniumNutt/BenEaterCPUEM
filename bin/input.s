#include "BECPU.cpu"


start:
	lda, #0x1
	sta, x
	lda, #0x0
loop:
	sta, y
	out
	lda, x
	add, y
	sta, x
	out
	lda, y
	add, x
	jc, start
	jmp, loop


x:
	#d8 0x0
y:
	#d8 0x0







