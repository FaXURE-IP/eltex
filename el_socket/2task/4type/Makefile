CC = gcc
CFLAGS = -Wall -pthread 
TARGETS = server udp_client tcp_client

all: $(TARGETS)

server: server.o
	$(CC) $(CFLAGS) server.o -o server

udp_client: udp_client.o
	$(CC) $(CFLAGS) udp_client.o -o udp_client

tcp_client: tcp_client.o
	$(CC) $(CFLAGS) tcp_client.o -o tcp_client


%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGETS) *.o
	rm -f server
	rm -f client

