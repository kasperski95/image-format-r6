#pragma once
#include <iostream>

template <typename T>
struct Point {
    T x;
    T y;

    Point(T xToSet = 0, T yToSet = 0) {
        x = xToSet;
        y = yToSet;
    }

    void print() {
        std::cout << "(" << x << ", " << y << ")";
    }
};
