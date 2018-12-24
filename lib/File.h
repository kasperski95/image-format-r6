#ifndef FILE_H_INCLUDED
#define FILE_H_INCLUDED

#include <string>

class File {
public:
    File(std::string filepath="");
    void filepath(std::string filepath);
    const char* path();
    const char* filename();
    const char* ext();
    bool initialized();

private:
    std::string _path;
    std::string _filename;
    std::string _ext;
};

#endif
