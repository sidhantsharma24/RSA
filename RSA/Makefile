# Compiler
CC = gcc
WINCC = gcc

# vcpkg installed directory (user-defined)
VCPKG_DIR ?= /c/vcpkg/installed/x64-windows

# Directories
SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build
OUTPUT_DIR = output

# Files
TARGET = rsa_program
TARGET_WIN = rsa_program.exe

SRC_FILES_COMMON = main.c $(SRC_DIR)/rsa.c $(SRC_DIR)/prime_gen.c
SRC_FILES_WIN = $(SRC_FILES_COMMON) $(SRC_DIR)/applink.c
SRC_FILES_LINUX = $(SRC_FILES_COMMON)

# Flags
CFLAGS = -Wall -Wextra -I$(INCLUDE_DIR) -I$(VCPKG_DIR)/include
LIBS = -L$(VCPKG_DIR)/lib -lmpir -lcrypto -lgmp
WINLIBS = $(LIBS)

# Output Binaries
BIN = $(BUILD_DIR)/$(TARGET)
BIN_WIN = $(BUILD_DIR)/$(TARGET_WIN)

# Detect OS and set default target
ifeq ($(OS),Windows_NT)
all: windows
else
all: $(BIN)
endif

# Windows Target
windows: $(BUILD_DIR) $(OUTPUT_DIR) $(BIN_WIN)
	@cp $(VCPKG_DIR)/bin/mpir.dll $(BUILD_DIR)
	@echo "[+] Copied mpir.dll to build directory."

# Build Linux
$(BIN): $(SRC_FILES_LINUX) | $(BUILD_DIR)
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)
	@echo "[+] Build complete: $(BIN)"

# Build Windows
$(BIN_WIN): $(SRC_FILES_WIN) | $(BUILD_DIR)
	$(WINCC) $(CFLAGS) $^ -o $@ $(WINLIBS)
	@echo "[+] Windows build complete: $(BIN_WIN)"

# Create directories
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(OUTPUT_DIR):
	mkdir -p $(OUTPUT_DIR)

# Clean
clean:
	rm -rf $(BUILD_DIR) $(OUTPUT_DIR)
	@echo "[+] Cleaned build and output directories."

.PHONY: all windows clean

