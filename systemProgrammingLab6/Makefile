CC=gcc
CFLAGS=-O2 -Wextra -Wall -Wno-unused-result -std=c99
TARGET=sys6.out
OBJS=syslab6.o

all: $(TARGET)

sys6.out: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

syslab6.c: Syslab6.c
	$(CC) $(CFLAGS) -c -o $@ $^
	
clean:
	rm *.o $(TARGET)
