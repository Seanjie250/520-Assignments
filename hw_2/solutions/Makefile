CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g
LDFLAGS = 

# Source files
SOLUTIONS_SRC = solutions.c
TEST_SRC = unit_tests.c
MAIN_SRC = main.c

# Object files
SOLUTIONS_OBJ = $(SOLUTIONS_SRC:.c=.o)
TEST_OBJ = $(TEST_SRC:.c=.o)
MAIN_OBJ = $(MAIN_SRC:.c=.o)

# Executables
TEST_EXEC = unit_tests
MAIN_EXEC = main

# Default target
all: $(TEST_EXEC) $(MAIN_EXEC)

# Build test executable
$(TEST_EXEC): $(TEST_OBJ) $(SOLUTIONS_OBJ)
	$(CC) $(CFLAGS) -o $(TEST_EXEC) $(TEST_OBJ) $(SOLUTIONS_OBJ) $(LDFLAGS)

# Build main executable
$(MAIN_EXEC): $(MAIN_OBJ) $(SOLUTIONS_OBJ)
	$(CC) $(CFLAGS) -o $(MAIN_EXEC) $(MAIN_OBJ) $(SOLUTIONS_OBJ) $(LDFLAGS)

# Compile object files
%.o: %.c solutions.h
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	rm -f $(SOLUTIONS_OBJ) $(TEST_OBJ) $(MAIN_OBJ) $(TEST_EXEC) $(MAIN_EXEC)

# Run tests
test: $(TEST_EXEC)
	./$(TEST_EXEC)

# Phony targets
.PHONY: all clean test

