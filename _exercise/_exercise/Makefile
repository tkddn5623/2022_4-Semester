CC=gcc
CFLAGS=-O2 -Wextra -Wall -Wno-unused-result -std=c99
TARGET=exercise
OBJS=exercise.o

all: $(TARGET)

exercise: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

exercise.o: exercise.c
	$(CC) $(CFLAGS) -c -o $@ $^
	
clean:
	rm *.o $(TARGET)
