CC = gcc
CFLAGS = -Wall -Werror
TARGETS = 2

all: $(TARGETS)

2: 2.o
	$(CC) $(CFLAGS) 2.o -o 2

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGETS) *.o
	rm -f 2

