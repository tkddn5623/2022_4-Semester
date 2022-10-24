CC=gcc
CFLAGS=-O2 -W -Wall -Wno-unused-result -std=c99
TARGET=sys9.out
OBJS=sys9.o

all: $(TARGET)

sys9.out: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

sys9.o: sys9.c
	$(CC) $(CFLAGS) -c -o $@ $^
	
clean:
	rm *.o $(TARGET)
