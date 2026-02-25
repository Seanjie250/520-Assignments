#ifndef TYPED_ARRAY_H
#define TYPED_ARRAY_H

#include <vector>
#include <stdexcept>
#include <algorithm>

template <typename ElementType>
class TypedArray {
private:
    std::vector<ElementType> elements;

public:
    TypedArray() {}
    
    TypedArray(const TypedArray& other) : elements(other.elements) {}
    
    TypedArray& operator=(const TypedArray& other) {
        if (this != &other) {
            elements = other.elements;
        }
        return *this;
    }
    
    size_t size() const {
        return elements.size();
    }
    
    ElementType& get(size_t index) {
        return elements[index];
    }
    
    const ElementType& get(size_t index) const {
        return elements[index];
    }
    
    void set(size_t index, const ElementType& value) {
        if (index >= elements.size()) {
            elements.resize(index + 1);
        }
        elements[index] = value;
    }
    
    void push(const ElementType& value) {
        elements.push_back(value);
    }
    
    ElementType pop() {
        if (elements.empty()) {
            throw std::range_error("Cannot pop from an empty array");
        }
        ElementType value = elements.back();
        elements.pop_back();
        return value;
    }
    
    void push_front(const ElementType& value) {
        elements.insert(elements.begin(), value);
    }
    
    ElementType pop_front() {
        if (elements.empty()) {
            throw std::range_error("Cannot pop from an empty array");
        }
        ElementType value = elements.front();
        elements.erase(elements.begin());
        return value;
    }
    
    TypedArray concat(const TypedArray& other) const {
        TypedArray result;
        result.elements.reserve(elements.size() + other.elements.size());
        result.elements.insert(result.elements.end(), elements.begin(), elements.end());
        result.elements.insert(result.elements.end(), other.elements.begin(), other.elements.end());
        return result;
    }
    
    TypedArray& reverse() {
        std::reverse(elements.begin(), elements.end());
        return *this;
    }
    
    TypedArray operator+(const TypedArray& other) const {
        return concat(other);
    }
};

#endif
