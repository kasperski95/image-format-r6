#pragma once

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
};
