CC=gcc
CFLAGS=-O2 -Wextra -Wall -Wno-unused-result -std=c99
TARGET=sys8.out
OBJS=sys8.o

all: $(TARGET)

sys8.out: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

sys8.o: sys8.c
	$(CC) $(CFLAGS) -c -o $@ $^
	
clean:
	rm *.o $(TARGET)
