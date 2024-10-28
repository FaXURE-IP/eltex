CC = gcc
CFLAGS = -Wall -pthread 
TARGETS = server

all: $(TARGETS)

server: server.o
	$(CC) $(CFLAGS) server.o -o server
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGETS) *.o
	rm -f server
	rm -f client

