CC=gcc
CFLAGS=-O2 -Wextra -Wall -Wno-unused-result -std=c99
TARGET=sys13
OBJS=sys13.o

all: $(TARGET)

sys13: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

sys13.o: sys13.c
	$(CC) $(CFLAGS) -c -o $@ $^
	
clean:
	rm *.o $(TARGET)
