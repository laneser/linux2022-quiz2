CC = gcc
CFLAGS = -O1 -g -Wall -Werror -Idudect -I.

all: test1 test2 test3 test4 test5 test6 test7

%.o: %.c
	$(VECHO) "  CC\t$@\n"
	$(Q)$(CC) -o $@ $(CFLAGS) -c -MMD -MF .$@.d $<

clean:
	rm -f test?

run: all
	./test1
	./test2
	./test3
	./test4
	./test5
	./test6
	./test7
