# Compiler
CC = gcc
WINCC = x86_64-w64-mingw32-gcc

# Directories
SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build
OUTPUT_DIR = output

# Files
TARGET = rsa_program
TARGET_WIN = rsa_program.exe

SRC_FILES = main.c $(SRC_DIR)/rsa.c $(SRC_DIR)/prime_gen.c

# Flags
CFLAGS = -Wall -Wextra -I$(INCLUDE_DIR)
LIBS = -lmpir -lcrypto -lgmp
WINLIBS = -lmpir -lcrypto -lgmp

# Output Binaries
BIN = $(BUILD_DIR)/$(TARGET)
BIN_WIN = $(BUILD_DIR)/$(TARGET_WIN)

# Default Target (Linux)
all: $(BUILD_DIR) $(OUTPUT_DIR) $(BIN)

# Windows Target
windows: $(BUILD_DIR) $(OUTPUT_DIR) $(BIN_WIN)

# Build Linux
$(BIN): $(SRC_FILES)
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)
	@echo "[+] Build complete: $(BIN)"

# Build Windows
$(BIN_WIN): $(SRC_FILES)
	$(WINCC) $(CFLAGS) $^ -o $@ $(WINLIBS)
	@echo "[+] Windows build complete: $(BIN_WIN)"

# Create build/output directories if they don't exist
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(OUTPUT_DIR):
	mkdir -p $(OUTPUT_DIR)

# Clean
clean:
	rm -rf $(BUILD_DIR) $(OUTPUT_DIR)
	@echo "[+] Cleaned build and output directories."

.PHONY: all windows clean

