#pragma once
#include <iostream>

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

    bool operator!= (Color<T> const &col) {
        return r != col.r || g != col.g || b != col.b;
    }

    Color<T>& operator+=(const Color<T> &rhs){
        r += rhs.r;
        g += rhs.g;
        b += rhs.b;
        a += rhs.a;
        return *this;
    }

    Color<T> operator*(float x) {
        r *= x;
        g *= x;
        b *= x;
        a *= x;
        return *this;
    }

    void print() {
        std::cout << "[" << r << " " << g << " " << b << "]";
    }
};
