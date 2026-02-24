#ifndef INTEGRATOR_H
#define INTEGRATOR_H

#include "elma/elma.h"

using namespace elma;

class Integrator : public Process {
public:
    Integrator(std::string name);
    void init() override;
    void update() override;
    double value();

private:
    double integral_;
};

#endif
