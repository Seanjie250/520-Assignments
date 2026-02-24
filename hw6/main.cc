#include "stopwatch.h"
#include "random_process.h"
#include "filter.h"
#include "integrator.h"
#include "elma/elma.h"
#include <iostream>
#include <thread>
#include <chrono>

using namespace elma;
using namespace std::chrono;

#define SLEEP std::this_thread::sleep_for(std::chrono::milliseconds(1500))

int main() {
    std::cout << "=== HW6 Assignment Demo ===" << std::endl;
    
    std::cout << "\n--- Part 1: Stopwatch Demo ---" << std::endl;
    Stopwatch w;
    w.start();
    SLEEP;
    w.stop();
    std::cout << "After 1.5s sleep: " << w.get_seconds() << " seconds" << std::endl;
    
    SLEEP;
    std::cout << "After another 1.5s (stopwatch stopped): " << w.get_seconds() << " seconds" << std::endl;
    
    w.start();
    SLEEP;
    w.stop();
    std::cout << "After another 1.5s (total): " << w.get_seconds() << " seconds" << std::endl;
    
    w.reset();
    std::cout << "After reset: " << w.get_seconds() << " seconds" << std::endl;
    
    std::cout << "\n--- Part 2: ELMA Process Composition Demo ---" << std::endl;
    
    Manager m;
    RandomProcess r("random numbers");
    Filter f("filter");
    Channel link("link");
    
    m.schedule(r, 1_ms)
     .schedule(f, 1_ms)
     .add_channel(link)
     .init()
     .run(100_ms);
    
    std::cout << "Filter running average after 100ms: " << f.value() << std::endl;
    
    std::cout << "\n--- Integrator Demo ---" << std::endl;
    Manager m2;
    RandomProcess r2("random numbers 2");
    Integrator i("integrator");
    Channel link2("link");
    
    m2.schedule(r2, 1_ms)
      .schedule(i, 1_ms)
      .add_channel(link2)
      .init()
      .run(100_ms);
    
    std::cout << "Integrator value after 100ms: " << i.value() << std::endl;
    
    std::cout << "\n=== Demo Complete ===" << std::endl;
    return 0;
}
