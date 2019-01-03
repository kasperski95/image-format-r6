#pragma once
#include <iostream>
#include <cmath>

struct Color {
    long r;
    long g;
    long b;
    long a;

    Color(long p_r = 0, long p_g = 0, long p_b = 0, long p_a = 255) {
        r = p_r;
        g = p_g;
        b = p_b;
        a = p_a;
    }

    void print() {
        std::cout << "[" << r << " " << g << " " << b << "]";
    }

    Color& saturation(float value) {
        long avg = (r + g + b) / 3;
        r = avg * (1 - value) + r * value;
        g = avg * (1 - value) + g * value;
        b = avg * (1 - value) + b * value;
        return *this;
    }

    bool operator!= (Color const &col) {
        return r != col.r || g != col.g || b != col.b;
    }

    Color& operator+=(const Color &rhs){
        r += rhs.r;
        g += rhs.g;
        b += rhs.b;
        return *this;
    }

    Color operator*(double x) {
        return Color(round(r * x), round(g * x), round(b * x));
    }

    Color operator-(const Color &rhs) {
        return Color(r - rhs.r, g - rhs.g, b - rhs.b);
    }

    Color operator+(const Color &rhs) {
        return Color(r + rhs.r, g + rhs.g, b + rhs.b);
    }

    bool operator<(const Color &rhs) {
        return (abs(r)*0.299 + abs(g)*0.587 + abs(b)*0.114) < (abs(rhs.r)*0.299 + abs(rhs.g)*0.587 + abs(rhs.b)*0.114);
    }
};
