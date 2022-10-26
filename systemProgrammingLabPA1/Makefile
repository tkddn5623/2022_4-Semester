CC=gcc
CFLAGS=-O2 -Wextra -Wall -Wno-unused-result -std=c99
TARGET=minishell
OBJS=syspa1.o

all: $(TARGET)

minishell: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

syspa1.o: syspa1.c
	$(CC) $(CFLAGS) -c -o $@ $^
	
clean:
	rm *.o $(TARGET)
