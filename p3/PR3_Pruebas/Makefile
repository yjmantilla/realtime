# Compiler for x86 Ubuntu
CXX_UBUNTU = g++

# Compiler for Raspberry Pi 4
CXX_RPI4 = aarch64-rpi4-linux-gnu-g++

# Source files
SOURCES = main.cpp Prey.cpp Predator.cpp TimeSpec.cpp

# Output binary names
OUT_UBUNTU = predatorPreyX86-ubuntu
OUT_RPI4 = predatorPreyARM

# Compiler flags (add any desired flags here)
CXXFLAGS = -Wall -std=c++11

.PHONY: all ubuntu rpi4 clean

# Default target
all: ubuntu rpi4

# Target for x86 Ubuntu
ubuntu:
	$(CXX_UBUNTU) $(CXXFLAGS) $(SOURCES) -o $(OUT_UBUNTU)

# Target for Raspberry Pi 4
rpi4:
	$(CXX_RPI4) $(CXXFLAGS) $(SOURCES) -o $(OUT_RPI4)

# Clean target to remove output binaries
clean:
	rm -f $(OUT_UBUNTU) $(OUT_RPI4)

