C_FLAGS = -Wall -O2
CC = gcc

SRC_DIR = src/

assembler : label lexer parser
	$(CC) $(C_FLAGS) -o assembler \
	$(SRC_DIR)label.o $(SRC_DIR)lexer.o $(SRC_DIR)parser.o \
	$(SRC_DIR)assembler.c

label : $(SRC_DIR)label.c
	$(CC) $(C_FLAGS) -c $(SRC_DIR)label.c -o $(SRC_DIR)label.o

lexer : $(SRC_DIR)lexer.c
	$(CC) $(C_FLAGS) -c $(SRC_DIR)lexer.c -o $(SRC_DIR)lexer.o

parser : $(SRC_DIR)parser.c
	$(CC) $(C_FLAGS) -c $(SRC_DIR)parser.c -o $(SRC_DIR)parser.o

clean : 
	rm $(SRC_DIR)*.o
	rm assembler
