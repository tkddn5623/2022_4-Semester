CC=gcc
CFLAGS=-O2 -W -Wall -Wno-unused-result -std=c99
TARGET=sys10.out
OBJS=sys10.o

all: $(TARGET)

sys10.out: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

sys10.o: sys10.c
	$(CC) $(CFLAGS) -c -o $@ $^
	
clean:
	rm *.o $(TARGET)
