CC=gcc
CFLAGS=-O2 -Wextra -Wall -Wno-unused-result -std=c99
TARGET=sys7.out
OBJS=syslab7.o

all: $(TARGET)

sys7.out: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

syslab7.o: syslab7.c
	$(CC) $(CFLAGS) -c -o $@ $^
	
clean:
	rm *.o $(TARGET)
