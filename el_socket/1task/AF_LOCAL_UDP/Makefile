CC = gcc
CFLAGS = -Wall -pthread 
TARGETS = server client

all: $(TARGETS)

server: server.o
	$(CC) $(CFLAGS) server.o -o server

client: client.o
	$(CC) $(CFLAGS) client.o -o client


%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGETS) *.o
	rm -f server
	rm -f client

