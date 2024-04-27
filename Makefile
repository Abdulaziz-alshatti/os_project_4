# Define the compiler
CXX = g++

# Compiler flags
CXXFLAGS = -Wall -Wextra -std=c++17 -pthread

# Define the source files
SOURCES = main.cpp MemoryManager.cpp Process.cpp

# Define the executable file 
EXECUTABLE = mem_sim

# Default target
all: $(EXECUTABLE)

# Rule to link the object files into an executable
$(EXECUTABLE): $(SOURCES)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $@

# Rule to clean old builds
clean:
	rm -f $(EXECUTABLE)

# Add a phony target to prevent problems with local files
.PHONY: clean
