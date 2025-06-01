CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -fstack-protector-all
SRC_DIR = src
OBJ_DIR = output
RESULT_DIR = result
PROJECT_PATH := $(shell pwd)

# Find all .c files in the source directory
SRC_FILES := $(wildcard $(SRC_DIR)/*.c)
# Generate corresponding .o file names
OBJ_FILES := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC_FILES))

# Main executable
TARGET = program

# Create obj and result directories if they don't exist
$(shell mkdir -p $(OBJ_DIR))
$(shell mkdir -p $(RESULT_DIR))

# Default target
all: $(TARGET)

# Link object files to create executable
$(TARGET): $(OBJ_FILES)
	$(CC) $(CFLAGS) -o $@ $^

# Compile source files into object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@


# Clean target
clean:
	rm -rf $(OBJ_DIR) $(TARGET)
	rm -rf ${RESULT_DIR}

test1: 
	./program -o "./${RESULT_DIR}" -e "./input" -f "00-vaso-simples.geo" -q "clone.qry"

test2:
	./program -o "./${RESULT_DIR}" -e "./input" -f "01-band-suecia.geo"

debug:
	valgrind --leak-check=full --track-origins=yes ./program -o "./${RESULT_DIR}" -e "./input" -f "01-band-suecia.geo"

.PHONY: all clean
