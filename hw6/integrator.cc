#include "integrator.h"

Integrator::Integrator(std::string name) 
    : Process(name), integral_(0.0) {
}

void Integrator::init() {
    integral_ = 0.0;
}

void Integrator::update() {
    if (channel("link").nonempty()) {
        double v = channel("link").latest();
        integral_ += delta() * v;
    }
}

double Integrator::value() {
    return integral_;
}
