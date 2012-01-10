C_FLAGS = -Wall -O2
CC = gcc

SRC_DIR = src/

all : vm assembler disassembler

vm : $(SRC_DIR)debug.o $(SRC_DIR)image.o $(SRC_DIR)vm.o $(SRC_DIR)god_mode.o
	$(CC) $(C_FLAGS) -o vm \
	$(SRC_DIR)debug.o $(SRC_DIR)image.o $(SRC_DIR)vm.o $(SRC_DIR)god_mode.o \
	-lncurses

disassembler : $(SRC_DIR)disassembler.o $(SRC_DIR)debug.o
	$(CC) $(C_FLAGS) -o disassembler \
	$(SRC_DIR)disassembler.o $(SRC_DIR)debug.o

assembler : $(SRC_DIR)label.o $(SRC_DIR)lexer.o $(SRC_DIR)parser.o \
			$(SRC_DIR)memory_definition.o $(SRC_DIR)parameter_list.o \
			$(SRC_DIR)assembler.o
	$(CC) $(C_FLAGS) -o assembler \
	$(SRC_DIR)label.o $(SRC_DIR)lexer.o $(SRC_DIR)parser.o $(SRC_DIR)memory_definition.o \
	$(SRC_DIR)assembler.o $(SRC_DIR)parameter_list.c

$(SRC_DIR)assembler.o : $(SRC_DIR)assembler.c
	$(CC) $(C_FLAGS) -c $(SRC_DIR)assembler.c -o $(SRC_DIR)assembler.o

$(SRC_DIR)disassembler.o : $(SRC_DIR)disassembler.c
	$(CC) $(C_FLAGS) -c $(SRC_DIR)disassembler.c -o $(SRC_DIR)disassembler.o

$(SRC_DIR)debug.o : $(SRC_DIR)debug.c $(SRC_DIR)debug.h
	$(CC) $(C_FLAGS) -c $(SRC_DIR)debug.c -o $(SRC_DIR)debug.o

$(SRC_DIR)god_mode.o : $(SRC_DIR)god_mode.c $(SRC_DIR)god_mode.h
	$(CC) $(C_FLAGS) -c $(SRC_DIR)god_mode.c -o $(SRC_DIR)god_mode.o

$(SRC_DIR)label.o : $(SRC_DIR)label.c $(SRC_DIR)label.h
	$(CC) $(C_FLAGS) -c $(SRC_DIR)label.c -o $(SRC_DIR)label.o

$(SRC_DIR)lexer.o : $(SRC_DIR)lexer.c $(SRC_DIR)lexer.h
	$(CC) $(C_FLAGS) -c $(SRC_DIR)lexer.c -o $(SRC_DIR)lexer.o
	
$(SRC_DIR)image.o : $(SRC_DIR)image.c $(SRC_DIR)image.h
	$(CC) $(C_FLAGS) -c $(SRC_DIR)image.c -o $(SRC_DIR)image.o

$(SRC_DIR)memory_definition.o : $(SRC_DIR)memory_definition.c $(SRC_DIR)memory_definition.h
	$(CC) $(C_FLAGS) -c $(SRC_DIR)memory_definition.c -o $(SRC_DIR)memory_definition.o

$(SRC_DIR)parameter_list.o : $(SRC_DIR)parameter_list.c $(SRC_DIR)parameter_list.h
	$(CC) $(C_FLAGS) -c $(SRC_DIR)parameter_list.c -o $(SRC_DIR)parameter_list.o

$(SRC_DIR)parser.o : $(SRC_DIR)parser.c $(SRC_DIR)parser.h
	$(CC) $(C_FLAGS) -c $(SRC_DIR)parser.c -o $(SRC_DIR)parser.o

$(SRC_DIR)vm.o : $(SRC_DIR)vm.c $(SRC_DIR)vm.h
	$(CC) $(C_FLAGS) -c $(SRC_DIR)vm.c -o $(SRC_DIR)vm.o

clean : 
	rm $(SRC_DIR)*.o
	rm assembler
	rm vm
	rm disassembler
