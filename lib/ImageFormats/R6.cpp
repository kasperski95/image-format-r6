#include <stdint.h>
#include <fstream>
#include <bitset>
#include <cmath>
#include <sys/stat.h>
#include "R6.h"


R6::R6() {
    // default mode
    this->mode = Mode::DEDICATED;

    // generate fixed palette
    int r, g, b;
    std::vector<int> values {0, 85, 170, 255};
    for (int i = 0; i < 64; ++i) {
        r = (i & 0b00000011);
        g = (i & 0b00001100) >> 2;
        b = (i & 0b00110000) >> 4;
        _palette.push_back(Color(values[r], values[g], values[b]));
    }

    // generate grayscale palette
    for (int i = 0; i < 62; i++) {
        _grayscale.push_back(Color(i*4, i*4, i*4));
    }
    _grayscale.push_back(Color(249,249,249));
    _grayscale.push_back(Color(255,255,255));
}


// CORE
unsigned int R6::load(Filepath &filepath, ImageBuffer* buffer) {
    std::ifstream file(filepath.raw(), std::ios::binary);
    if (!file) throw bad_source();

    Header header;
    file.read(reinterpret_cast<char*>(&header), sizeof(header));

    // verify header
    if (header.version != '1') throw bad_filesize();

    // verify file size
    struct stat st;
    stat(filepath.raw(), &st);
    if (st.st_size != header.fileSize) throw bad_filesize();


    switch(header.mode) {
        case 1: this->mode = Mode::DEDICATED; break;
        case 2: this->mode = Mode::FIXED; break;
        case 3: this->mode = Mode::GRAYSCALE; break;
    }

    // read palette
    switch(mode) {
        case Mode::DEDICATED:
            for (int i = 0; i < header.nPaletteImportantColors; ++i) {
            char r = file.get();
            char g = file.get();
            char b = file.get();
            buffer->palette(Color(r, g, b));
        }
        break;
        case Mode::FIXED:
            buffer->palette(_palette);
        break;
        case Mode::GRAYSCALE:
            buffer->palette(_grayscale);
        break;
    }


    // read indexMatrix
    buffer->init(header.width, header.height);
    file.seekg(header.offset);

    char c;
    std::string bitstring;
    while(file.get(c))
        bitstring += std::bitset<8>((unsigned char)c).to_string();

    int index, x = 0, y = 0;
    for (int i = 0; i < bitstring.size() - 6; i += 6) {
        index = 0;

        for (int j = 0; j < 6; ++j) {
            index += (bitstring[i+j] - '0') * pow(2, 5 - j);
        }

        buffer->index(x, y, index);
        x++;
        if (x >= header.width) {
            x = 0;
            y++;
        }
    }

    buffer->generateBuffer();

    return header.fileSize;
}


unsigned int R6::save(Filepath &filepath, ImageBuffer* buffer) {
    std::ofstream file(filepath.raw(), std::ios::binary);

    if (file) {
        // prepare header
        Header header;
        header.version = '1';

        switch(mode) {
            case Mode::DEDICATED: header.mode = 1; break;
            case Mode::FIXED: header.mode = 2; break;
            case Mode::GRAYSCALE: header.mode = 3; break;
        }

        header.offset = sizeof(Header);
        header.nPaletteImportantColors = 0;
        if (mode == Mode::DEDICATED) {
            header.nPaletteImportantColors = buffer->paletteSize();
            header.offset = sizeof(Header) + 64 * 3;
        }

        header.width = buffer->width();
        header.height = buffer->height();

        // prepare data
        // TODO: Rice algorithm
        std::string bitstring;
        int index;
        for (int y = 0; y < buffer->height(); ++y) {
            for (int x = 0; x < buffer->width(); ++x) {
                index = buffer->index(x, y);
                for (int i = 5; i >= 0; --i) {
                    bitstring += (index >> i) % 2 + '0';
                }
            }
        }
        while(bitstring.size() % 8)
            bitstring += '0';

        // calculate file size
        header.fileSize = header.offset + (bitstring.length() / 8);

        // SAVE
        {
            // save header
            file.write(reinterpret_cast<char*>(&header), sizeof(header));

            // prepare & save dedicated palette
            if (mode == Mode::DEDICATED) {
                unsigned char r, g, b;
                for (int i = 0; i < buffer->paletteSize(); ++i) {
                    r = (unsigned char)buffer->palette(i).r;
                    g = (unsigned char)buffer->palette(i).g;
                    b = (unsigned char)buffer->palette(i).b;
                    file.put(r);
                    file.put(g);
                    file.put(b);
                }
                for (int i = 0; i < 64 - buffer->paletteSize(); ++i) {
                    file.put(0);
                    file.put(0);
                    file.put(0);
                }
            }

            // save data
            _saveBitstring(file, bitstring);
        }

        file.close();

        return header.fileSize;
    }

    return 0;
}


// GETTERS
const std::vector<Color>& R6::palette() {
    if (mode == Mode::GRAYSCALE)
        return _grayscale;
    return _palette;
};

//=================================================================

void R6::_saveBitstring(std::ofstream &file, std::string &bitstring) {
    // pad with zeroes to make it represent an integral multiple of bytes
    while(bitstring.size() % 8)
        bitstring += '0';

    unsigned char b;
    for(int i = 0; i < bitstring.size(); i += 8) {
        b = std::bitset<8>(bitstring.substr(i, 8)).to_ulong();
        file.put(b);
    }
}
