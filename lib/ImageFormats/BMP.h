#pragma once
#include "_ImageFormat.h"
#include <string>



class BMP : public ImageFormat{
public:
    BMP();

    // CORE
    unsigned int load(Filepath &filepath, ImageBuffer* buffer) override;
    unsigned int save(Filepath &filepath, ImageBuffer* buffer) override;

private:
    std::string _fileHeader;
    std::string _infoHeader;
    const int _bytesPerPixel = 3;

    //-----------------------------------------------------------------

    void _createFileHeader(ImageBuffer* buffer, int paddingSize);
    void _createInfoHeader(ImageBuffer* buffer);
};
