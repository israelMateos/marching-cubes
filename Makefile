# Directories
SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build
BIN_DIR = bin

# Output executable
OUTPUT = $(BIN_DIR)/marching_cubes

# Source and object files
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES = $(SRC_FILES:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

# Compiler and flags
CC = gcc
CFLAGS = -O2 -Wall -I $(INCLUDE_DIR)

# Targets
.PHONY: all clean

# Default target to build the project
all: $(OUTPUT)

# Building the output executable from object files
$(OUTPUT): $(OBJ_FILES)
	@mkdir -p $(dir $@) # Ensure the output directory exists
	$(CC) $(OBJ_FILES) -o $(OUTPUT)

# Compiling source files into object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@) # Ensure the build directory exists
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)
