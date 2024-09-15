CC = gcc
CFLAGS = -Wall -Werror
TARGETS = 1 2_server 2_client 

all: $(TARGETS)

1: 1.o
	$(CC) $(CFLAGS) 1.o -o 1

2_server: 2_server.o
	$(CC) $(CFLAGS) 2_server.o -o 2_server

2_client: 2_client.o
	$(CC) $(CFLAGS) 2_client.o -o 2_client

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGETS) *.o
	rm -f 1
	rm -f 2_server
	rm -f 2_client

