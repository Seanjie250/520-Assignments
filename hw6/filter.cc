#include "filter.h"

Filter::Filter(std::string name) 
    : Process(name), running_average_(0.0) {
}

void Filter::init() {
    values_.clear();
    running_average_ = 0.0;
}

void Filter::update() {
    if (channel("link").nonempty()) {
        double new_value = channel("link").latest();
        values_.push_back(new_value);
        if (values_.size() > WINDOW_SIZE) {
            values_.pop_front();
        }
        double sum = 0.0;
        for (double v : values_) {
            sum += v;
        }
        running_average_ = sum / values_.size();
    }
}

double Filter::value() {
    return running_average_;
}
