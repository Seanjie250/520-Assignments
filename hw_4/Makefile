CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++14 -g
LDFLAGS = -lm

SRC = matrix.cc
TEST_SRC = unit_tests.cc
MAIN_SRC = main.cc

OBJ = $(SRC:.cc=.o)
TEST_OBJ = $(TEST_SRC:.cc=.o)
MAIN_OBJ = $(MAIN_SRC:.cc=.o)

TEST_EXEC = unit_tests
MAIN_EXEC = main

all: $(TEST_EXEC) $(MAIN_EXEC)

$(TEST_EXEC): $(TEST_OBJ) $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(TEST_EXEC) $(TEST_OBJ) $(OBJ) $(LDFLAGS)

$(MAIN_EXEC): $(MAIN_OBJ) $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(MAIN_EXEC) $(MAIN_OBJ) $(OBJ) $(LDFLAGS)

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TEST_OBJ) $(MAIN_OBJ) $(TEST_EXEC) $(MAIN_EXEC)

test: $(TEST_EXEC)
	./$(TEST_EXEC)

.PHONY: all clean test
