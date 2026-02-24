#ifndef STOPWATCH_H
#define STOPWATCH_H

#include <chrono>
#include <thread>

class Stopwatch {
public:
    Stopwatch();
    void start();
    void stop();
    void reset();
    double get_minutes();
    double get_seconds();
    double get_milliseconds();
    double get_nanoseconds();

private:
    std::chrono::high_resolution_clock::time_point start_time_;
    std::chrono::high_resolution_clock::time_point stop_time_;
    std::chrono::nanoseconds elapsed_time_;
    bool is_running_;
};

#endif
