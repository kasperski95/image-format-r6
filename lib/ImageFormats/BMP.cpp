#include <Windows.h>
#include <fstream>
#include <cassert>
#include "_ImageFormat.h"
#include "BMP.h"



BMP::BMP() :
    _fileHeader(14, 0),
    _infoHeader(40, 0)
{}


// CORE
unsigned int BMP::load(Filepath &filepath, ImageBuffer* buffer) {
    std::ifstream file(filepath.raw(), std::ios::binary);
    if (file) {
        BITMAPFILEHEADER bmFileHeader;
        file.read(reinterpret_cast<char*>(&bmFileHeader), sizeof(bmFileHeader));

        BITMAPINFOHEADER bmInfoHeader;
        file.read(reinterpret_cast<char*>(&bmInfoHeader), sizeof(bmInfoHeader));

        //TODO: handle more depths
        assert(bmInfoHeader.biBitCount == 24);
        assert(bmInfoHeader.biCompression == BI_RGB);

        buffer->init(bmInfoHeader.biWidth, bmInfoHeader.biHeight);

        file.seekg(bmFileHeader.bfOffBits);
        const int padding = (4 - (buffer->width() * 3) % 4) % 4;
        for (int y = buffer->height() - 1; y >= 0; --y) {
            for (int x = 0; x < buffer->width(); ++x) {
                buffer->px(x, y, Color(file.get(), file.get(), file.get()));
            }
            file.seekg(padding, std::ios::cur);
        }
        return bmFileHeader.bfSize;
    }
    return 0;
}


unsigned int BMP::save(Filepath &filepath, ImageBuffer* buffer) {
    std::ofstream file(filepath.raw(), std::ios::binary);

    if (file) {
        int padding = (4 - (buffer->width() * _bytesPerPixel) % 4) % 4;

        _createFileHeader(buffer, padding);
        _createInfoHeader(buffer);

        file << _fileHeader;
        file << _infoHeader;

        for (int y = buffer->height() - 1; y >= 0; --y) {
            for (int x = 0; x < buffer->width(); ++x) {
                file << (char)buffer->px(x,y).b << (char)buffer->px(x,y).g << (char)buffer->px(x,y).r;
            }
            for (int x = 0; x < padding; ++x) {
                file << 0;
            }
        }
        file.close();
        return _fileHeader.size() + _infoHeader.size() + (_bytesPerPixel* buffer->width() + padding) * buffer->height();
    }

    return 0;
}

//=================================================================

void BMP::_createFileHeader(ImageBuffer* buffer, int padding) {
    int fileSize = _fileHeader.size() + _infoHeader.size() + (_bytesPerPixel* buffer->width() + padding) * buffer->height();

    _fileHeader[0] = 'B';
    _fileHeader[1] = 'M';

    _fileHeader[2] = (unsigned char)(fileSize);
    _fileHeader[3] = (unsigned char)(fileSize >> 8);
    _fileHeader[4] = (unsigned char)(fileSize >> 16);
    _fileHeader[5] = (unsigned char)(fileSize >> 24);

    _fileHeader[10] = (unsigned char)(_fileHeader.size() + _infoHeader.size());
}


void BMP::_createInfoHeader(ImageBuffer* buffer){
    _infoHeader[0] = (unsigned char)(_infoHeader.size());
    _infoHeader[4] = (unsigned char)(buffer->width());
    _infoHeader[5] = (unsigned char)(buffer->width() >> 8);
    _infoHeader[6] = (unsigned char)(buffer->width() >> 16);
    _infoHeader[7] = (unsigned char)(buffer->width() >> 24);
    _infoHeader[8] = (unsigned char)(buffer->height());
    _infoHeader[9] = (unsigned char)(buffer->height() >> 8);
    _infoHeader[10] = (unsigned char)(buffer->height() >> 16);
    _infoHeader[11] = (unsigned char)(buffer->height() >> 24);
    _infoHeader[12] = (unsigned char)(1);
    _infoHeader[14] = (unsigned char)(_bytesPerPixel * 8);
}
