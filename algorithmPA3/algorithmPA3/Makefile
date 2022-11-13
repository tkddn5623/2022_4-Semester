CC=gcc
CFLAGS=-O2 -Wextra -Wall -Wno-unused-result -std=c99
TARGET=a.out
OBJS=algorithmPA3.o

all: $(TARGET)

a.out: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

algorithmPA3.o: algorithmPA3.c
	$(CC) $(CFLAGS) -c -o $@ $^
	
clean:
	rm *.o $(TARGET)
