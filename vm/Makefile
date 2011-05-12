C_FLAGS = -Wall -O2
CC = gcc

SRC_DIR = src/

main : $(SRC_DIR)debug $(SRC_DIR)image $(SRC_DIR)vm
	$(CC) $(C_FLAGS) -o vm \
	$(SRC_DIR)debug.o $(SRC_DIR)image.o $(SRC_DIR)vm.o \
	$(SRC_DIR)main.c

$(SRC_DIR)debug : $(SRC_DIR)debug.c
	$(CC) $(C_FLAGS) -c $(SRC_DIR)debug.c -o $(SRC_DIR)debug.o

$(SRC_DIR)image : $(SRC_DIR)image.c
	$(CC) $(C_FLAGS) -c $(SRC_DIR)image.c -o $(SRC_DIR)image.o

$(SRC_DIR)vm : $(SRC_DIR)vm.c
	$(CC) $(C_FLAGS) -c $(SRC_DIR)vm.c -o $(SRC_DIR)vm.o

clean : 
	rm $(SRC_DIR)*.o
	rm vm
