# Compiler
CC := g++

# Compiler flags
CFLAGS := -std=c++11 -Wall -Wextra -Werror -pedantic -march=native -O3 -ftree-vectorize -fopt-info-vec

# Libraries
LIBS := -lglut -lGL -lGLU

# Source files
SRCS := HeatField.cpp main.cpp

# Object files
OBJS := $(SRCS:.cpp=.o)

# Executable name
EXEC := boiling-2d

# Rule to build the executable
$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

# Rule to compile source files
%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

# Rule to clean the project
clean:
	rm -f $(OBJS) $(EXEC)
