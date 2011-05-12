C_FLAGS = -Wall -O2
CC = gcc

BUILD_DIR = o/
SRC_DIR = src/

main : $(BUILD_DIR)debug $(BUILD_DIR)image $(BUILD_DIR)vm
	$(CC) $(C_FLAGS) -o vm \
	$(BUILD_DIR)debug.o $(BUILD_DIR)image.o $(BUILD_DIR)vm.o \
	$(SRC_DIR)main.c

$(BUILD_DIR)debug : $(SRC_DIR)debug.c
	$(CC) $(C_FLAGS) -c $(SRC_DIR)debug.c -o $(BUILD_DIR)debug.o

$(BUILD_DIR)image : $(SRC_DIR)image.c
	$(CC) $(C_FLAGS) -c $(SRC_DIR)image.c -o $(BUILD_DIR)image.o

$(BUILD_DIR)vm : $(SRC_DIR)vm.c
	$(CC) $(C_FLAGS) -c $(SRC_DIR)vm.c -o $(BUILD_DIR)vm.o

clean : 
	rm $(BUILD_DIR)*
	rm vm
