#include <stdint.h>
#include <fstream>
#include <bitset>
#include <cmath>
#include <sys/stat.h>
#include <iostream>
#include "../compression-util.h"
#include "R6.h"


R6::R6() {
    // default mode
    this->mode = Mode::DEDICATED;
    this->dithering = false;

    // generate fixed palette
    int r, g, b;
    std::vector<int> values {0, 85, 170, 255};
    for (int i = 0; i < 64; ++i) {
        r = (i & 0b00000011);
        g = (i & 0b00001100) >> 2;
        b = (i & 0b00110000) >> 4;
        _palette.push_back(Color(values[r], values[g], values[b]).saturation(0.8));
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

    this->mode = (Mode)header.mode;

    // read palette
    for (int i = 0; i < header.nColors; ++i) {
        char r = file.get();
        char g = file.get();
        char b = file.get();
        buffer->palette(Color(r, g, b));
    }

    // read indexMatrix
    buffer->init(header.width, header.height);
    file.seekg(header.offset);

    char c;
    std::string bitstring;
    while(file.get(c))
        bitstring += std::bitset<8>((unsigned char)c).to_string();

    int index, x = 0, y = 0, offset = 0;
    while( y < header.height ){
        int prefix_end = bitstring.find('0', offset);
        std::string next_value = bitstring.substr(offset, prefix_end + 4 - offset);
        index = rice_decode(8, next_value);
        offset += prefix_end + 4 - offset;
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
        header.dithering = this->dithering;
        header.mode = (uint8_t)this->mode;
        header.offset = sizeof(Header) + 64 * 3;
        header.nColors = buffer->paletteSize();
        header.width = buffer->width();
        header.height = buffer->height();

        // prepare data
        std::string bitstring = "";
        int index;
        for (int y = 0; y < buffer->height(); ++y) {
            for (int x = 0; x < buffer->width(); ++x) {
                index = buffer->index(x, y);
                bitstring += rice_encode(8, index);
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

            // save palette
            {
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


void R6::print(Filepath &filepath) {
    using namespace std;
    ifstream file(filepath.raw(), ios::binary);
    if (!file) throw bad_source();

    Header header;
    file.read(reinterpret_cast<char*>(&header), sizeof(header));

    // verify header
    if (header.version == '1') {
        cout << "format version: " << header.version << endl;

        cout << "mode: ";
        switch(header.mode) {
            case 0: cout << "dedicated palette"; break;
            case 1: cout << "fixed palette"; break;
            case 2: cout << "grayscale"; break;
            default: cout << "unknown mode: " << to_string(header.mode);
        }
        cout << endl;

        cout << "dithering: ";
        if (header.dithering) cout << "true"; else cout << "false";
        cout << endl;

        if (header.mode == (uint8_t)Mode::DEDICATED)
            cout << "used colors: " << to_string(header.nColors) << endl;

        cout << "offset: " << header.offset << " bytes" << endl;
        cout << "file size: " << header.fileSize << " bytes" << endl;
        cout << "width: " << header.width << " px" << endl;
        cout << "height: " << header.height << " px" << endl;
    }
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
