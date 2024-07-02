OBJ_DIR=.
SOURCES=$(shell find ./* -name '*.c')

EXECUTABLE01=pr01
EXECUTABLE02=pr02
EXECUTABLE03=pr03

all: $(EXECUTABLE01) pr01.o $(EXECUTABLE02) pr02.o $(EXECUTABLE03) pr03.o

lib:
	$(MAKE) -C ../el_bits
	$(MAKE) -C ../el_mas
	$(MAKE) -C ../el_structure


$(EXECUTABLE01): pr01.o
	gcc pr01.o  -L../el_bits -lbits -o pr01

pr01.o: pr01.c lib 
	gcc pr01.c -I../include -c -o pr01.o 

$(EXECUTABLE02): pr02.o
	gcc pr02.o  -L../el_mas -lmas -o pr02

pr02.o: pr02.c lib 
	gcc pr02.c -I../include -c -o pr02.o 

$(EXECUTABLE03): pr03.o
	gcc pr03.o  -L../el_structure -lstructure -o pr03

pr03.o: pr03.c lib 
	gcc pr03.c -I../include -c -o pr03.o 


clean: 
	find $(OBJ_DIR) -name '*.o' -exec $(RM) '{}' \;
	find $(OBJ_DIR) -name '*.d' -exec $(RM) '{}' \;
	$(RM) $(EXECUTABLE)

.PHONY: all clean lib