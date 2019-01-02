#pragma once
#include <iostream>
#include <cmath>

template <typename T>
struct Color {
    T r;
    T g;
    T b;
    T a;

    Color(T p_r = 0, T p_g = 0, T p_b = 0, T p_a = 255) {
        r = p_r;
        g = p_g;
        b = p_b;
        a = p_a;
    }

    void print() {
        std::cout << "[" << r << " " << g << " " << b << "]";
    }

    bool operator!= (Color<T> const &col) {
        return r != col.r || g != col.g || b != col.b;
    }

    Color<T>& operator+=(const Color<T> &rhs){
        r += rhs.r;
        g += rhs.g;
        b += rhs.b;
        return *this;
    }

    Color<T> operator*(double x) {
        return Color<T>(round(r * x), round(g * x), round(b * x));
    }

    Color<T> operator-(const Color<T> &rhs) {
        return Color<T>(r - rhs.r, g - rhs.g, b - rhs.b);
    }

    Color<T> operator+(const Color<T> &rhs) {
        return Color<T>(r + rhs.r, g + rhs.g, b + rhs.b);
    }

    bool operator<(const Color<T> &rhs) {
        return (abs(r) + abs(g) + abs(b)) < (abs(rhs.r) + abs(rhs.g) + abs(rhs.b));
    }
};
