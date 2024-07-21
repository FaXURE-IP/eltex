OBJ_DIR=.
SOURCES=$(shell find ./* -name '*.c')

EXECUTABLE01=bits
EXECUTABLE02=mas
EXECUTABLE03=structure
EXECUTABLE04=FM
EXECUTABLE05=calc

all: $(EXECUTABLE01) bits.o $(EXECUTABLE02) mas.o $(EXECUTABLE03) structure.o $(EXECUTABLE04) FM.o $(EXECUTABLE05) calc.o

lib:
	$(MAKE) -C ../el_bits
	$(MAKE) -C ../el_mas
	$(MAKE) -C ../el_structure
	$(MAKE) -C ../el_FM
	$(MAKE) -C ../el_calculate


$(EXECUTABLE01): bits.o
	gcc bits.o  -L../el_bits -lbits -o bits

bits.o: bits.c lib 
	gcc bits.c -I../include -c -o bits.o 

$(EXECUTABLE02): mas.o
	gcc mas.o  -L../el_mas -lmas -o mas

mas.o: mas.c lib 
	gcc mas.c -I../include -c -o mas.o 

$(EXECUTABLE03): structure.o
	gcc structure.o  -L../el_structure -lstructure -o structure

structure.o: structure.c lib 
	gcc structure.c -I../include -c -o structure.o 

$(EXECUTABLE04): FM.o
	gcc FM.o  -L../el_FM -lFM -lncursesw -o FM

FM.o: FM.c lib 
	gcc FM.c -I../include -c -o FM.o 

$(EXECUTABLE05): calc.o
	gcc calc.o -L../el_calculate -lmenu -Wl,-rpath,../el_calculate -o calc

calc.o: calc.c lib 
	gcc calc.c -I../include  -L../el_calculate -lmenu -c -o calc.o 


clean: 
	find $(OBJ_DIR) -name '*.o' -exec $(RM) '{}' \;
	find $(OBJ_DIR) -name '*.d' -exec $(RM) '{}' \;
	$(RM) $(EXECUTABLE)

.PHONY: all clean lib