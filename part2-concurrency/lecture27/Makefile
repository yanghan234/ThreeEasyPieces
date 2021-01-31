CC=gcc
SRCS=$(wildcard *.c)
OBJS=$(patsubst %.c,%.o,$(SRCS))
CFLAGS=-Wall -pthread

all: $(OBJS)

%.o : %.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -rf *.o
