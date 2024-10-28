CC=gcc
CFLAGS= -Wall -Werror -Wextra
LDFLAGS=
OBJ_DIR=obj
SOURCES=$(shell find ./* -name '*.c')

OBJECTS=$(SOURCES:./%.c=$(OBJ_DIR)/%.o)


all:
	$(MAKE) -C ./el_bits
	$(MAKE) -C ./el_mas
	$(MAKE) -C ./el_pointer
	$(MAKE) -C ./el_FM
	$(MAKE) -C ./el_structure
	$(MAKE) -C ./el_calculate
	$(MAKE) -C ./el_process
	$(MAKE) -C ./el_threads
	$(MAKE) -C ./el_socket
	$(MAKE) -C ./el_queue
	$(MAKE) -C ./el_sem
	$(MAKE) -C ./el_signal
	$(MAKE) -C ./el_pipes

clean: 
	find . -name '*.o' -exec $(RM) '{}' \;
	find . -name '*.d' -exec $(RM) '{}' \;
	find . -name '*.a' -exec $(RM) '{}' \;

.PHONY: all clean