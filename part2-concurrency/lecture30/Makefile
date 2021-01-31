CC=gcc
SRCS=$(wildcard *.c)
OBJS=$(patsubst %.c,%.o,$(SRCS))
CFLAGS=-Wall -Og

all: $(OBJS)

%.o : %.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -rf *.o
