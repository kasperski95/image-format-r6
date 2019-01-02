#pragma once
#include <string>



class Filepath {
public:
    Filepath(std::string filepath="");

    // CORE
    void init(std::string filepath);

    // GETTERS
    const char* path() const;
    const char* filename() const;
    const char* ext() const;
    const char* raw() const;
    bool initialized();

    // SETTERS
    void ext(const char* newExt);

private:
    std::string _path;
    std::string _filename;
    std::string _ext;
    std::string _raw;
};
