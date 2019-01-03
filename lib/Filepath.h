#pragma once
#include <string>



class Filepath {
public:
    Filepath(std::string filepath="");

    // CORE
    void init(std::string filepath);

    // GETTERS
    std::string path() const;
    std::string filename() const;
    std::string ext() const;
    const char* raw() const;
    bool initialized();

    // SETTERS
    void ext(std::string newExt);

private:
    std::string _path;
    std::string _filename;
    std::string _ext;
    std::string _raw;
};
