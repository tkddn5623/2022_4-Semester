CC=gcc
CFLAGS=-O2 -Wextra -Wall -Wno-unused-result -std=c99
TARGET=pa2
OBJS=algorithmPA2.o

all: $(TARGET)

pa2: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

algorithmPA2.o: algorithmPA2.c
	$(CC) $(CFLAGS) -c -o $@ $^
	
clean:
	rm *.o $(TARGET)