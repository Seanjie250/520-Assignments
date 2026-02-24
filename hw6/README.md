# HW6 Assignment

This assignment implements three parts:

1. **Stopwatch Class** - A high-precision timer for measuring execution time
2. **Process Composition** - Random number generation, filtering, and integration using the ELMA framework
3. **LLM Tool Use Agent** - A C++ application integrating with OpenAI's GPT API using function calling

## Files

- `stopwatch.h` / `stopwatch.cc` - Stopwatch class implementation
- `random_process.h` / `random_process.cc` - RandomProcess ELMA process
- `filter.h` / `filter.cc` - Filter ELMA process
- `integrator.h` / `integrator.cc` - Integrator ELMA process
- `llm_agent.cpp` - LLM Tool Use Agent implementation
- `main.cc` - Main demonstration program
- `unit_tests.cc` - Unit tests for all components
- `Makefile` - Build configuration

## Prerequisites

- C++14 compatible compiler (g++ recommended)
- ELMA framework installed (https://github.com/klavinslab/elma)
- nlohmann/json library (included with ELMA)
- httplib library (included with ELMA)
- OpenAI API key (for Part 3)

## Building

```bash
make
```

This will build:
- `unit_tests` - Test executable
- `main` - Main demonstration program
- `llm_agent` - LLM agent executable

## Running

### Part 1 & 2: Stopwatch and ELMA Processes

```bash
./main
```

Or run unit tests:

```bash
make test
# or
./unit_tests
```

### Part 3: LLM Tool Use Agent

Set your OpenAI API key:

**Option 1: Use the helper script (for this session only)**
```bash
source set_api_key.sh
```

**Option 2: Set manually**
```bash
export OPENAI_API_KEY="sk-your-key-here"
```

**Note:** The API key is stored in `set_api_key.sh` for convenience. For security, consider:
- Adding `set_api_key.sh` to `.gitignore` if using version control
- Using environment variables or a secure credential manager in production

Run the agent:

```bash
./llm_agent
```

The agent will execute three test queries:
1. "What's 2847 * 3921?"
2. "Find the mean and standard deviation of: 23, 45, 67, 12, 89, 34, 56, 78"
3. "Generate the first 20 Fibonacci numbers, then calculate their mean and median."

## Implementation Details

### Part 1: Stopwatch

The Stopwatch class uses `std::chrono::high_resolution_clock` for nanosecond-precision timing. It supports:
- Starting and stopping the timer
- Accumulating elapsed time across multiple start/stop cycles
- Resetting to zero
- Getting time in minutes, seconds, milliseconds, or nanoseconds

### Part 2: ELMA Processes

- **RandomProcess**: Generates random doubles between 0 and 1 and sends them to a "link" channel
- **Filter**: Maintains a running average of the last 10 values received from the "link" channel
- **Integrator**: Numerically integrates values from the "link" channel using the formula: `integral += delta() * value`

### Part 3: LLM Tool Use Agent

The agent implements:
- **Tool Registry**: Manages available tools (calculator, statistics, Fibonacci)
- **Agent Loop**: Communicates with OpenAI API, handles tool calls, and maintains conversation history
- **Multi-step Reasoning**: Supports chaining multiple tool calls to solve complex queries

Tools implemented:
- **Calculator**: Arithmetic operations (add, subtract, multiply, divide)
- **Statistics**: Mean, standard deviation, and median calculations
- **Fibonacci**: Generates Fibonacci number sequences

## Testing

The unit tests verify:
- Stopwatch accuracy and accumulation behavior
- Filter running average calculation
- Integrator numerical integration

Run tests with:

```bash
make test
```

## Cleanup

```bash
make clean
```

## Notes

- The LLM agent uses the `gpt-5-nano` model as specified
- Ensure ELMA framework is properly installed and linked
- For the LLM agent, ensure your OpenAI API key has access to the gpt-5-nano model
- The agent includes error handling for invalid inputs and missing tools
