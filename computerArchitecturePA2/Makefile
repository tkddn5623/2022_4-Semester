CC=gcc
CFLAGS=-O2 -Wextra -Wall -Wno-unused-result -std=c99
TARGET=simul
OBJS=simul.o

all: $(TARGET)

simul: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

simul.o: simul.c
	$(CC) $(CFLAGS) -c -o $@ $^
	
clean:
	rm *.o $(TARGET)