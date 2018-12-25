#pragma once
#include "_ImageFormat.h"
#include <string>

class BMP : public ImageFormat{
public:
    BMP();
    void load(Filepath const &filepath, ImageBuffer* buffer) override;
    void save(Filepath const &filepath, ImageBuffer* buffer) override;

private:
    void _createFileHeader(ImageBuffer* buffer, int paddingSize);
    void _createInfoHeader(ImageBuffer* buffer);
    std::string _fileHeader;
    std::string _infoHeader;
    const int _bytesPerPixel = 3;
};
