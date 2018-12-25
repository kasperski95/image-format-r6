#pragma once
#include <vector>
#include "../Color.h"

using colorType = unsigned int;

class ImageBuffer {
public:
    ImageBuffer(int width=0, int height=0);
    void init(int width, int height);
    Color<colorType> px(int x, int y);
    void px(int x, int y, Color<colorType> color);
    void width(int width);
    void height(int height);
    int width();
    int height();
private:
    std::vector<std::vector<Color<colorType>>> _buffer;
    int _width;
    int _height;
};
