#include "BECPU.cpu"

	
top:
	lda, number
	add, number
	sta, number
	out
	jmp, top 

number:
	#d8 1
