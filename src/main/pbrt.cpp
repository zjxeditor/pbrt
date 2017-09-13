#include <iostream>
#include <cmath>
#include <algorithm>
#include "transform.h"

using namespace pbrt;

template<typename T>
inline bool isNaN(const T x) {
    return std::isnan(x);
}

template<>
inline bool isNaN(const int x) {
    return false;
}

template<typename T>
T adder(T v) {
    return v;
}

template<typename T, typename... Args>
T adder(T first, Args... args) {
    return first + adder(args...);
}


int main() {
    Matrix4x4 m(1,3,2,1,2,1,1,4,3,2,1,2,2,4,3,1);
    Matrix4x4 invM = Inverse(m);
    std::cout << invM << std::endl;
}