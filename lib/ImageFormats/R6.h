#pragma once
#include <exception>
#include <vector>
#include <fstream>
#include "_ImageFormat.h"



class R6 : public ImageFormat {
public:
    struct Header {
        uint8_t version;
        uint8_t mode;
        uint8_t dithering;
        uint8_t nColors;
        uint32_t offset;
        uint32_t fileSize;
        uint32_t width;
        uint32_t height;
    };

    enum class Mode {
        DEDICATED,
        FIXED,
        GRAYSCALE
    };
    Mode mode;
    bool dithering;
    //-----------------------------------------------------------------

    R6();

    // CORE
    unsigned int load(Filepath &filepath, ImageBuffer* buffer) override;
    unsigned int save(Filepath &filepath, ImageBuffer* buffer) override;

    void print(Filepath &filepath);

    // GETTERS
    const std::vector<Color>& palette();


private:
    std::vector<Color> _palette;
    std::vector<Color> _grayscale;

    //-----------------------------------------------------------------

    void _saveBitstring(std::ofstream &file, std::string &bitstring);
};
