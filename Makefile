# Makefile for OpenCubMan

# Compiler
CC = g++

# Compiler Flags
CFLAGS = -O2 -g -fconserve-space

# Source files
SOURCES = cvtofe.cpp fetocv.cpp opencubman.cpp

# Header files
HEADERS = opencubman.h opencubman_typdef.h

# Object files
OBJECTS = opencubman.o cvtofe.o fetocv.o

# Executables to build
EXECS = cvtofe fetocv

# Default target to build all executables
all: $(EXECS)

# Build 'cvtofe' executable
cvtofe: cvtofe.o opencubman.o
	$(CC) $(CFLAGS) cvtofe.o opencubman.o -lm -o cvtofe
	@echo "Built cvtofe"

# Build 'fetocv' executable
fetocv: fetocv.o opencubman.o
	$(CC) $(CFLAGS) fetocv.o opencubman.o -lm -o fetocv
	@echo "Built fetocv"

# Compile source files into object files
%.o: %.cpp $(HEADERS)
	@echo "Compiling $<"
	$(CC) $(CFLAGS) -c $<

# Clean up build files and executables
clean:
	@echo "Deleting $(OBJECTS) and executables $(EXECS)"
	rm -f $(OBJECTS) $(EXECS)

# Phony targets
.PHONY: all clean
