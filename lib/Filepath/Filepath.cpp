#include "../Filepath.h"
#include <string>
#include <fstream>
#include <functional> //bad_function_call
#include <iostream>
#include <bitset>

using namespace std;


Filepath::Filepath(string filepath) {
    if (!filepath.empty())
        this->init(filepath);
}

void Filepath::init(string filepath) {
    int endOfPath = filepath.find_last_of("/\\");
    int endOfFilename = filepath.find_first_of(".");

    if(endOfFilename == -1) {
        throw bad_function_call();
    }

    _raw = filepath;

    if (endOfPath != -1) {
        _path = filepath.substr(0, endOfPath);
        _filename = filepath.substr(endOfPath + 1, endOfFilename - endOfPath - 1);
        _ext = filepath.substr(endOfFilename + 1);
    } else {
        _path.clear();
        _filename = filepath.substr(0, endOfFilename);
        _ext = filepath.substr(endOfFilename + 1);
    }

}


void Filepath::readBits(std::string &output) {
    std::ifstream file(_raw, std::ios::binary);

    char c;
    while(file.get(c))
        output += std::bitset<8>((unsigned char)c).to_string();
}


void Filepath::writeBits(std::string bitstring) {
    // pad with zeroes to make it represent an integral multiple of bytes
    while(bitstring.size() % 8)
        bitstring += '0';

    std::ofstream file(_raw, std::ios::binary);

    if (file) {
        unsigned char b;
        for(int i = 0; i < bitstring.size(); i += 8) {
            b = std::bitset<8>(bitstring.substr(i, 8)).to_ulong();
            file.put(b);
            //file << b;
        }
        file.close();
    }
}


const char* Filepath::path() const {return _path.c_str();}
const char* Filepath::filename() const {return _filename.c_str();}
const char* Filepath::ext() const {return _ext.c_str();}
const char* Filepath::raw() const {return _raw.c_str();}
bool Filepath::initialized() {return !_raw.empty();}

void Filepath::ext(const char* newExt) {
    _ext = newExt;
    _raw = _path + _filename + "." + _ext;
}
