#include <stdint.h>
#include <fstream>
#include <bitset>
#include <cmath>
#include "../Filepath.h"
#include "R6.h"


R6::R6() {
    this->mode = Mode::DEDICATED;
}


void R6::load(Filepath &filepath, ImageBuffer* buffer) {
    std::ifstream file(filepath.raw(), std::ios::binary);

    Header header;
    file.read(reinterpret_cast<char*>(&header), sizeof(header));


    // read palette
    for (int i = 0; i < header.paletteSize; ++i) {
        buffer->palette(Color<colorType>(file.get(), file.get(), file.get()));
    }

    // read indexMatrix
    buffer->init(header.width, header.height);
    file.seekg(header.offset);

    char c;
    std::string bitstring;
    while(file.get(c))
        bitstring += std::bitset<8>((unsigned char)c).to_string();

    {
        int index, x = 0, y = 0;
        for (int i = 0; i < bitstring.size() - 7; i += 7) {
            index = 0;

            for (int j = 1; j < 7; ++j) {
                index += (bitstring[i+j] - '0') * pow(2, 6 - j);
            }

            buffer->index(x, y, index);
            x++;
            if (x >= header.width) {
                x = 0;
                y++;
            }
        }
    }

    buffer->updateBuffer();

}


void R6::save(Filepath &filepath, ImageBuffer* buffer) {
    std::ofstream file(filepath.raw(), std::ios::binary);

    if (file) {
        Header header;
        header.version = 1;
        header.mode = 1;
        header.offset = sizeof(Header) + (64 * 3);
        header.paletteSize = buffer->paletteSize();
        header.fileSize = 0; //TODO
        header.width = buffer->width();
        header.height = buffer->height();

        file.write(reinterpret_cast<char*>(&header), sizeof(header));

        // (palette [64 * 3])
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

        // data (Rice algorithm)
        // palette size is fixed: 64 and m = 64 => 0 + <binary code>
        std::string bitstring;
        int index;
        switch(mode) {
        case Mode::DEDICATED:
            for (int y = 0; y < buffer->height(); ++y) {
                for (int x = 0; x < buffer->width(); ++x) {
                    bitstring += '0';
                    index = buffer->index(x, y);
                    for (int i = 5; i >= 0; --i) {
                        bitstring += (index >> i) % 2 + '0';
                    }
                }
            }
            //std::cout << bitstring;

            {
                // pad with zeroes to make it represent an integral multiple of bytes
                while(bitstring.size() % 8)
                    bitstring += '0';

                unsigned char b;
                for(int i = 0; i < bitstring.size(); i += 8) {
                    b = std::bitset<8>(bitstring.substr(i, 8)).to_ulong();
                    file.put(b);
                }

            }
        break;
        }

        file.close();

    }
}



