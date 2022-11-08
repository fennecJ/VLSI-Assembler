CC := gcc
CCFLAG := -std=c11
objs := ins.o
target := assembler.o
.PHONY: clean
all: $(target)

%.o: %.c %.h
	$(CC) $(CCFLAG) -c $<

$(target): %.o: %.c $(objs)
	$(CC) $(CCFLAG) -o asm -g $^

clean :
	rm -f *.o *.gch asm
