CC=gcc
CFLAGS= -Wall -Werror -Wextra
LDFLAGS=
OBJ_DIR=obj
SOURCES=$(shell find ./* -name '*.c')

OBJECTS=$(SOURCES:./%.c=$(OBJ_DIR)/%.o)


all:
	$(MAKE) -C ./console
	$(MAKE) -C ./el_bits
	$(MAKE) -C ./el_mas
	$(MAKE) -C ./el_pointer

clean: 
	find . -name '*.o' -exec $(RM) '{}' \;
	find . -name '*.d' -exec $(RM) '{}' \;
	find . -name '*.a' -exec $(RM) '{}' \;

.PHONY: all clean