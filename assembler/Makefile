C_FLAGS = -Wall -O2
CC = gcc

BUILD_DIR = o/
SRC_DIR = src/

assembler : label lexer parser
	$(CC) $(C_FLAGS) -o assembler \
	$(BUILD_DIR)label.o $(BUILD_DIR)lexer.o $(BUILD_DIR)parser.o \
	$(SRC_DIR)assembler.c

label : $(SRC_DIR)label.c
	$(CC) $(C_FLAGS) -c $(SRC_DIR)label.c -o $(BUILD_DIR)label.o

lexer : $(SRC_DIR)lexer.c
	$(CC) $(C_FLAGS) -c $(SRC_DIR)lexer.c -o $(BUILD_DIR)lexer.o

parser : $(SRC_DIR)parser.c
	$(CC) $(C_FLAGS) -c $(SRC_DIR)parser.c -o $(BUILD_DIR)parser.o

clean : 
	rm $(BUILD_DIR)*
	rm assembler
