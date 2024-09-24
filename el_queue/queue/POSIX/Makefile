CC = gcc
CFLAGS = -Wall -pthread 
TARGETS = client server

all: $(TARGETS)

client: client.o
	$(CC) $(CFLAGS) client.o -o client

server: server.o
	$(CC) $(CFLAGS) server.o -o server

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGETS) *.o
	rm -f client
	rm -f server
