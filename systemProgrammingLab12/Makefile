CC=gcc
CFLAGS=-O2 -Wextra -Wall -Wno-unused-result -std=c99
TARGET=server client

all: $(TARGET)

server: sys12_server.o
	$(CC) $(CFLAGS) -o $@ $^

client: sys12_client.o
	$(CC) $(CFLAGS) -o $@ $^

sys12_server.o: sys12_server.c
	$(CC) $(CFLAGS) -c -o $@ $^
	
sys12_client.o: sys12_client.c
	$(CC) $(CFLAGS) -c -o $@ $^

clean:
	rm *.o $(TARGET)
