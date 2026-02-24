#ifndef FILTER_H
#define FILTER_H

#include "elma/elma.h"
#include <deque>

using namespace elma;

class Filter : public Process {
public:
    Filter(std::string name);
    void init() override;
    void update() override;
    double value();

private:
    std::deque<double> values_;
    double running_average_;
    static const int WINDOW_SIZE = 10;
};

#endif
