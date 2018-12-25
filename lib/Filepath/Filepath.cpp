#include "../Filepath.h"
#include <string>
#include <functional> //bad_function_call

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

const char* Filepath::path() const {return _path.c_str();}
const char* Filepath::filename() const {return _filename.c_str();}
const char* Filepath::ext() const {return _ext.c_str();}
const char* Filepath::raw() const {return _raw.c_str();}
bool Filepath::initialized() {return !_raw.empty();}
