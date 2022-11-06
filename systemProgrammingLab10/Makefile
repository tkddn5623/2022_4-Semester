CC=gcc
CFLAGS=-O2 -Wextra -Wall -Wno-unused-result -std=c99
TARGET=server client
OBJS1=sys10_server.o
OBJS2=sys10_client.o

all: $(TARGET)

server: $(OBJS1)
	$(CC) $(CFLAGS) -o $@ $^

client: $(OBJS2)
	$(CC) $(CFLAGS) -o $@ $^

sys10_server.o: sys10_server.c
	$(CC) $(CFLAGS) -c -o $@ $^

sys10_client.o: sys10_client.c
	$(CC) $(CFLAGS) -c -o $@ $^
	
clean:
	rm *.o $(TARGET)
