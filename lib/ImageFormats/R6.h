#pragma once
#include <exception>
#include "_ImageFormat.h"

class R6 : public ImageFormat {
    struct Header {
        uint8_t version;
        uint8_t mode;
        uint8_t offset;
        uint8_t paletteSize;
        uint32_t fileSize;
        uint32_t width;
        uint32_t height;
    };
public:
    R6();

    enum class Mode {
        DEDICATED,
        FIXED,
        GRAYSCALE
    };
    Mode mode;
    void load(Filepath &filepath, ImageBuffer* buffer) override;
    void save(Filepath &filepath, ImageBuffer* buffer) override;


};
