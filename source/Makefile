CC=gcc
CFLAGS=-Wall -Wextra -Wshadow -Wuninitialized -Werror -std=c99
OBJS=utility.o main.o
PROGNAME=zife
STATICNAME=zife_static

all: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(PROGNAME)

.PHONY: static
static: $(OBJS)
	$(CC) $(CFLAGS) -static $(OBJS) -o $(STATICNAME)

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

utility.o: utility.c utility.h
	$(CC) $(CFLAGS) -c utility.c

.PHONY: clean
clean:
	rm *.o
	if [ -f $(PROGNAME) ]; then rm $(PROGNAME); fi
	if [ -f $(STATICNAME) ]; then rm $(STATICNAME); fi
