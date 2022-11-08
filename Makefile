CC := gcc
CCFLAG := -std=c11
objs := ins.o sym_table.o
target := assembler.o
.PHONY: clean
all: $(target)

%.o: %.c %.h
	$(CC) $(CCFLAG) -c $<

$(target): %.o: %.c $(objs)
	$(CC) $(CCFLAG) -o asm -g $^

clean :
	rm -f *.o *.gch asm __TMP_ASM_PARSE_FILE
