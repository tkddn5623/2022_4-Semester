CC=gcc
CFLAGS=-O2 -Wextra -Wall -Wno-unused-result -std=c99
TARGET=syspa2
OBJS=syspa2.o

all: $(TARGET)

syspa2: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

syspa2.o: syspa2.c
	$(CC) $(CFLAGS) -c -o $@ $^
	
clean:
	rm *.o $(TARGET)