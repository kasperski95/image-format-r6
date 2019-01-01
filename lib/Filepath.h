#ifndef FILE_H_INCLUDED
#define FILE_H_INCLUDED

#include <string>

class Filepath {
public:
    Filepath(std::string filepath="");
    void init(std::string filepath);
    void readBits(std::string &output);
    void writeBits(std::string bitstring);

    const char* path() const;
    const char* filename() const;
    const char* ext() const;
    const char* raw() const;
    void ext(const char* newExt);
    bool initialized();

private:
    std::string _path;
    std::string _filename;
    std::string _ext;
    std::string _raw;
};

#endif
