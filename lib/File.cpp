#include "File.h"
#include <string>
#include <functional> //bad_function_call

using namespace std;

File::File(string filepath) {
    if (!filepath.empty())
        this->filepath(filepath);
}

void File::filepath(string filepath) {
    int endOfPath = filepath.find_last_of("/\\");
    int endOfFilename = filepath.find_first_of(".");

    if(endOfFilename == -1) {
        throw bad_function_call();
    }

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

const char* File::path() {return _path.c_str();}
const char* File::filename() {return _filename.c_str();}
const char* File::ext() {return _ext.c_str();}
bool File::initialized() {return !_path.empty() || !_filename.empty() || !_ext.empty();}
