#include "random_process.h"

RandomProcess::RandomProcess(std::string name) 
    : Process(name), dist_(0.0, 1.0) {
    rng_.seed(std::chrono::system_clock::now().time_since_epoch().count());
}

void RandomProcess::init() {
}

void RandomProcess::update() {
    double random_value = dist_(rng_);
    channel("link").send(random_value);
}
