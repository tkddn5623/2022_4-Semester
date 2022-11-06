CC=gcc
CFLAGS=-O2 -Wextra -Wall -Wno-unused-result -std=c99
TARGET=Aladdin.out
OBJS=Aladdin.o

all: $(TARGET)

Aladdin.out: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

aladdin.o: aladdin.c
	$(CC) $(CFLAGS) -c -o $@ $^
	
clean:
	rm *.o $(TARGET)
