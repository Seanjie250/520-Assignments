#include "stopwatch.h"
#include "random_process.h"
#include "filter.h"
#include "integrator.h"
#include "elma/elma.h"
#include <iostream>
#include <cassert>
#include <thread>
#include <chrono>
#include <cmath>

using namespace elma;
using namespace std::chrono;

#define SLEEP std::this_thread::sleep_for(std::chrono::milliseconds(1500))
#define EPSILON 0.1

void test_stopwatch() {
    std::cout << "Testing Stopwatch..." << std::endl;
    
    Stopwatch w;
    assert(std::abs(w.get_seconds()) < EPSILON);
    
    w.start();
    SLEEP;
    w.stop();
    double elapsed = w.get_seconds();
    assert(elapsed > 1.4 && elapsed < 1.6);
    
    SLEEP;
    double still_elapsed = w.get_seconds();
    assert(std::abs(still_elapsed - elapsed) < EPSILON);
    
    w.start();
    SLEEP;
    w.stop();
    double total = w.get_seconds();
    assert(total > 2.9 && total < 3.1);
    
    w.reset();
    assert(std::abs(w.get_seconds()) < EPSILON);
    
    std::cout << "  ✓ Stopwatch tests passed" << std::endl;
}

class AlternatingProcess : public Process {
public:
    AlternatingProcess(std::string name) : Process(name), toggle_(false) {}
    
    void init() override {}
    
    void update() override {
        double value = toggle_ ? 0.75 : 0.25;
        channel("link").send(value);
        toggle_ = !toggle_;
    }
    
private:
    bool toggle_;
};

void test_filter() {
    std::cout << "Testing Filter..." << std::endl;
    
    Manager m;
    AlternatingProcess alt("alternating");
    Filter f("filter");
    Channel link("link");
    
    m.schedule(alt, 1_ms)
     .schedule(f, 1_ms)
     .add_channel(link)
     .init()
     .run(100_ms);
    
    double avg = f.value();
    assert(std::abs(avg - 0.5) < EPSILON);
    
    std::cout << "  ✓ Filter tests passed (average: " << avg << ")" << std::endl;
}

class ConstantProcess : public Process {
public:
    ConstantProcess(std::string name, double value) 
        : Process(name), constant_value_(value) {}
    
    void init() override {}
    
    void update() override {
        channel("link").send(constant_value_);
    }
    
private:
    double constant_value_;
};

void test_integrator() {
    std::cout << "Testing Integrator..." << std::endl;
    
    Manager m;
    double constant = 2.0;
    ConstantProcess c("constant", constant);
    Integrator i("integrator");
    Channel link("link");
    
    m.schedule(c, 1_ms)
     .schedule(i, 1_ms)
     .add_channel(link)
     .init()
     .run(100_ms);
    
    double integral = i.value();
    assert(integral > 0.1 && integral < 0.3);
    
    std::cout << "  ✓ Integrator tests passed (integral: " << integral << ")" << std::endl;
}

int main() {
    std::cout << "=== Running Unit Tests ===" << std::endl;
    
    try {
        test_stopwatch();
        test_filter();
        test_integrator();
        
        std::cout << "\n=== All Tests Passed! ===" << std::endl;
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Test failed: " << e.what() << std::endl;
        return 1;
    }
}
