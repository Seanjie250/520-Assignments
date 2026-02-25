CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g
LDFLAGS = -lm

SRC = dynamic_array.c
TEST_SRC = test_dynamic_array.c
MAIN_SRC = main.c

OBJ = $(SRC:.c=.o)
TEST_OBJ = $(TEST_SRC:.c=.o)
MAIN_OBJ = $(MAIN_SRC:.c=.o)

TEST_EXEC = test_dynamic_array
MAIN_EXEC = main

all: $(TEST_EXEC) $(MAIN_EXEC)

$(TEST_EXEC): $(TEST_OBJ) $(OBJ)
	$(CC) $(CFLAGS) -o $(TEST_EXEC) $(TEST_OBJ) $(OBJ) $(LDFLAGS)

$(MAIN_EXEC): $(MAIN_OBJ) $(OBJ)
	$(CC) $(CFLAGS) -o $(MAIN_EXEC) $(MAIN_OBJ) $(OBJ) $(LDFLAGS)

%.o: %.c dynamic_array.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TEST_OBJ) $(MAIN_OBJ) $(TEST_EXEC) $(MAIN_EXEC)

test: $(TEST_EXEC)
	./$(TEST_EXEC)

.PHONY: all clean test
