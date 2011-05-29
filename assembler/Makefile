C_FLAGS = -Wall -g -O0
CC = gcc

SRC_DIR = src/

assembler : $(SRC_DIR)label.o $(SRC_DIR)lexer.o $(SRC_DIR)parser.o \
			$(SRC_DIR)memory_definition.o $(SRC_DIR)parameter_list.o \
			$(SRC_DIR)assembler.o
	$(CC) $(C_FLAGS) -o assembler \
	$(SRC_DIR)label.o $(SRC_DIR)lexer.o $(SRC_DIR)parser.o $(SRC_DIR)memory_definition.o \
	$(SRC_DIR)assembler.o $(SRC_DIR)parameter_list.c

$(SRC_DIR)label.o : $(SRC_DIR)label.c $(SRC_DIR)label.h
	$(CC) $(C_FLAGS) -c $(SRC_DIR)label.c -o $(SRC_DIR)label.o

$(SRC_DIR)assembler.o : $(SRC_DIR)assembler.c
	$(CC) $(C_FLAGS) -c $(SRC_DIR)assembler.c -o $(SRC_DIR)assembler.o

$(SRC_DIR)lexer.o : $(SRC_DIR)lexer.c $(SRC_DIR)lexer.h
	$(CC) $(C_FLAGS) -c $(SRC_DIR)lexer.c -o $(SRC_DIR)lexer.o

$(SRC_DIR)parser.o : $(SRC_DIR)parser.c $(SRC_DIR)parser.h
	$(CC) $(C_FLAGS) -c $(SRC_DIR)parser.c -o $(SRC_DIR)parser.o

$(SRC_DIR)memory_definition.o : $(SRC_DIR)memory_definition.c $(SRC_DIR)memory_definition.h
	$(CC) $(C_FLAGS) -c $(SRC_DIR)memory_definition.c -o $(SRC_DIR)memory_definition.o

$(SRC_DIR)parameter_list.o : $(SRC_DIR)parameter_list.c $(SRC_DIR)parameter_list.h
	$(CC) $(C_FLAGS) -c $(SRC_DIR)parameter_list.c -o $(SRC_DIR)parameter_list.o

clean : 
	rm $(SRC_DIR)*.o
	rm assembler
