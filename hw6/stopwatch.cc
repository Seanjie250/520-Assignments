#include "stopwatch.h"

Stopwatch::Stopwatch() : elapsed_time_(0), is_running_(false) {
}

void Stopwatch::start() {
    if (!is_running_) {
        start_time_ = std::chrono::high_resolution_clock::now();
        is_running_ = true;
    }
}

void Stopwatch::stop() {
    if (is_running_) {
        stop_time_ = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(
            stop_time_ - start_time_);
        elapsed_time_ += duration;
        is_running_ = false;
    }
}

void Stopwatch::reset() {
    elapsed_time_ = std::chrono::nanoseconds(0);
    is_running_ = false;
}

double Stopwatch::get_minutes() {
    if (is_running_) {
        auto current_time = std::chrono::high_resolution_clock::now();
        auto current_elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(
            current_time - start_time_);
        auto total_ns = elapsed_time_ + current_elapsed;
        return total_ns.count() / 60000000000.0;
    }
    return elapsed_time_.count() / 60000000000.0;
}

double Stopwatch::get_seconds() {
    if (is_running_) {
        auto current_time = std::chrono::high_resolution_clock::now();
        auto current_elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(
            current_time - start_time_);
        auto total_ns = elapsed_time_ + current_elapsed;
        return total_ns.count() / 1000000000.0;
    }
    return elapsed_time_.count() / 1000000000.0;
}

double Stopwatch::get_milliseconds() {
    if (is_running_) {
        auto current_time = std::chrono::high_resolution_clock::now();
        auto current_elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(
            current_time - start_time_);
        auto total_ns = elapsed_time_ + current_elapsed;
        return total_ns.count() / 1000000.0;
    }
    return elapsed_time_.count() / 1000000.0;
}

double Stopwatch::get_nanoseconds() {
    if (is_running_) {
        auto current_time = std::chrono::high_resolution_clock::now();
        auto current_elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(
            current_time - start_time_);
        auto total_ns = elapsed_time_ + current_elapsed;
        return static_cast<double>(total_ns.count());
    }
    return static_cast<double>(elapsed_time_.count());
}
