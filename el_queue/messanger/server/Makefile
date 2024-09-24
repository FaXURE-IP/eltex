PROGRAM = server
BINDIR = ../../bin
SRCDIR = src
INCDIR = include

CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -I$(INCDIR)

SRCS = $(wildcard $(SRCDIR)/*.c)

OBJS = $(SRCS:$(SRCDIR)/%.c=$(SRCDIR)/%.o)

all: $(BINDIR)/$(PROGRAM)

$(BINDIR)/$(PROGRAM): $(OBJS)
	@mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) -o $@ $^

$(SRCDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(SRCDIR)/*.o $(BINDIR)/$(PROGRAM)