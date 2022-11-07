CC = gcc
CCFLAG = -std=c11

all: asm

asm: assembler.c ins.o
	$(CC) $(CCFLAG) -o asm -g assembler.c ins.o

ins.o: ins.c ins.h
	$(CC) $(CCFLAG) -c ins.c ins.h

clean :
	rm -f ins.o ins.h.gch asm

.PHONY: asm clean
