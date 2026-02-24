CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++14 -g -I/usr/local/include
LDFLAGS = -lelma -L/usr/local/lib

# Source files
STOPWATCH_SRC = stopwatch.cc
RANDOM_PROCESS_SRC = random_process.cc
FILTER_SRC = filter.cc
INTEGRATOR_SRC = integrator.cc
LLM_AGENT_SRC = llm_agent.cpp
TEST_SRC = unit_tests.cc
MAIN_SRC = main.cc

# Object files
STOPWATCH_OBJ = $(STOPWATCH_SRC:.cc=.o)
RANDOM_PROCESS_OBJ = $(RANDOM_PROCESS_SRC:.cc=.o)
FILTER_OBJ = $(FILTER_SRC:.cc=.o)
INTEGRATOR_OBJ = $(INTEGRATOR_SRC:.cc=.o)
LLM_AGENT_OBJ = $(LLM_AGENT_SRC:.cpp=.o)
TEST_OBJ = $(TEST_SRC:.cc=.o)
MAIN_OBJ = $(MAIN_SRC:.cc=.o)

# Executables
TEST_EXEC = unit_tests
MAIN_EXEC = main
LLM_EXEC = llm_agent

# Default target
all: $(TEST_EXEC) $(MAIN_EXEC)

# Build test executable
$(TEST_EXEC): $(TEST_OBJ) $(STOPWATCH_OBJ) $(RANDOM_PROCESS_OBJ) $(FILTER_OBJ) $(INTEGRATOR_OBJ)
	$(CXX) $(CXXFLAGS) -o $(TEST_EXEC) $(TEST_OBJ) $(STOPWATCH_OBJ) $(RANDOM_PROCESS_OBJ) $(FILTER_OBJ) $(INTEGRATOR_OBJ) $(LDFLAGS)

# Build main executable
$(MAIN_EXEC): $(MAIN_OBJ) $(STOPWATCH_OBJ) $(RANDOM_PROCESS_OBJ) $(FILTER_OBJ) $(INTEGRATOR_OBJ)
	$(CXX) $(CXXFLAGS) -o $(MAIN_EXEC) $(MAIN_OBJ) $(STOPWATCH_OBJ) $(RANDOM_PROCESS_OBJ) $(FILTER_OBJ) $(INTEGRATOR_OBJ) $(LDFLAGS)

# Build LLM agent executable
$(LLM_EXEC): $(LLM_AGENT_OBJ)
	$(CXX) $(CXXFLAGS) -o $(LLM_EXEC) $(LLM_AGENT_OBJ) $(LDFLAGS) -lssl -lcrypto -lpthread

# Compile object files
%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	rm -f $(STOPWATCH_OBJ) $(RANDOM_PROCESS_OBJ) $(FILTER_OBJ) $(INTEGRATOR_OBJ) $(LLM_AGENT_OBJ) $(TEST_OBJ) $(MAIN_OBJ) $(TEST_EXEC) $(MAIN_EXEC) $(LLM_EXEC)

# Run tests
test: $(TEST_EXEC)
	./$(TEST_EXEC)

# Phony targets
.PHONY: all clean test
