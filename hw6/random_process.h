#ifndef RANDOM_PROCESS_H
#define RANDOM_PROCESS_H

#include "elma/elma.h"
#include <random>

using namespace elma;

class RandomProcess : public Process {
public:
    RandomProcess(std::string name);
    void init() override;
    void update() override;

private:
    std::mt19937 rng_;
    std::uniform_real_distribution<double> dist_;
};

#endif
