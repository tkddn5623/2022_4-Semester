CC=gcc
CFLAGS=-O2 -W -Wall -Wno-unused-result -std=c99
TARGET=Syslab5.out
OBJS=Syslab5.o

all: $(TARGET)

Syslab5.out: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

Syslab5.o: Syslab5.c
	$(CC) $(CFLAGS) -c -o $@ $^
	
clean:
	rm *.o $(TARGET)
