# Compiler and compiler flags
CFLAGS = -Wall -pthread

# Executable names
TARGET_UBUNTU = main_ubuntu
TARGET_RPI4 = main_rpi4

# Source files
SRC = main.cpp task_definition.cpp thread_management.cpp

# Object files for each target
OBJ_UBUNTU = $(SRC:.cpp=_ubuntu.o)
OBJ_RPI4 = $(SRC:.cpp=_rpi4.o)

# Compiler for x86 Ubuntu
CXX_UBUNTU = g++

# Compiler for Raspberry Pi 4
CXX_RPI4 = aarch64-rpi4-linux-gnu-g++

# Default target (Ubuntu)
all: $(TARGET_UBUNTU)

# Target for Raspberry Pi 4
rpi4: $(TARGET_RPI4)

# Rule for linking Ubuntu
$(TARGET_UBUNTU): $(OBJ_UBUNTU)
	$(CXX_UBUNTU) $(CFLAGS) -o $@ $^

# Rule for linking Raspberry Pi 4
$(TARGET_RPI4): $(OBJ_RPI4)
	$(CXX_RPI4) $(CFLAGS) -o $@ $^

# Rule for compiling for Ubuntu
%_ubuntu.o: %.cpp
	$(CXX_UBUNTU) $(CFLAGS) -c $< -o $@

# Rule for compiling for Raspberry Pi 4
%_rpi4.o: %.cpp
	$(CXX_RPI4) $(CFLAGS) -c $< -o $@

# Clean target for removing compiled files
clean:
	rm -f $(TARGET_UBUNTU) $(OBJ_UBUNTU)
	rm -f $(TARGET_RPI4) $(OBJ_RPI4)
	rm -f *_log.csv

# Phony targets
.PHONY: all rpi4 clean
