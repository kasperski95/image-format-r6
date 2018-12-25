#include "ImageBuffer.h"


ImageBuffer::ImageBuffer(int widthToSet, int heightToSet) {
    this->init(widthToSet, heightToSet);
}

void ImageBuffer::init(int widthToSet, int heightToSet) {
    _width = widthToSet;
    _height = heightToSet;
    _buffer.resize(_width);

    for(int i = 0; i < _buffer.size(); ++i) {
        _buffer[i].resize(_height);
    }
}

void ImageBuffer::px(int x, int y, Color<colorType> color) {_buffer[x][y] = color;}
void ImageBuffer::width(int widthToSet) {_width = widthToSet;}
void ImageBuffer::height(int heightToSet) {_height = heightToSet;}

Color<colorType> ImageBuffer::px(int x, int y) {return _buffer[x][y];}
int ImageBuffer::width() {return _width;}
int ImageBuffer::height() {return _height;}

