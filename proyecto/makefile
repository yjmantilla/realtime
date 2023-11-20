# Compiler and compiler flags
CFLAGS = -Wall -pthread

# Executable name
TARGET = main

# Source files
SRC = main.cpp task_definition.cpp thread_management.cpp

# Object files
OBJ = $(SRC:.cpp=.o)

# Compiler for x86 Ubuntu
CXX_UBUNTU = g++

# Compiler for Raspberry Pi 4
CXX_RPI4 = aarch64-rpi4-linux-gnu-g++

# Default target (Ubuntu)
all: CC = $(CXX_UBUNTU)
all: $(TARGET)

# Target for Raspberry Pi 4
rpi4: CC = $(CXX_RPI4)
rpi4: $(TARGET)

# Rule for linking
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

# Rule for compiling
%.o: %.cpp
	$(CC) $(CFLAGS) -c $<

# Clean target for removing compiled files
clean:
	rm -f $(TARGET) $(OBJ)
	rm -f *_log.csv
# Phony targets
.PHONY: all rpi4 clean
