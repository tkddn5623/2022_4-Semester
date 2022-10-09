CC=gcc
CFLAGS=-O2 -W -Wall -Wno-unused-result -std=c99
TARGET=pa1.out
OBJS=algorithmPA1.o

all: $(TARGET)

pa1.out: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

algorithmPA1.o: algorithmPA1.c
	$(CC) $(CFLAGS) -c -o $@ $^
	
clean:
	rm *.o $(TARGET)
