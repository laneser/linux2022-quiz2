CC = gcc
CFLAGS = -O1 -g -Wall -Werror -Idudect -I.

all: test1

%.o: %.c
	$(VECHO) "  CC\t$@\n"
	$(Q)$(CC) -o $@ $(CFLAGS) -c -MMD -MF .$@.d $<

clean:
	rm -f test1

run: all
	./test1
