CC=gcc
CFLAGS=-O2 -Wextra -Wall -Wno-pointer-to-int-cast -Wno-int-conversion -std=c99 -lpthread
TARGET=sys11
OBJS=sys11.o

all: $(TARGET)

sys11: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

sys11.o: sys11.c
	$(CC) $(CFLAGS) -c -o $@ $^
	
clean:
	rm *.o $(TARGET)