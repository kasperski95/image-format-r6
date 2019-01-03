#pragma once
#include "../Filepath.h"
#include "../DataStructures/Color.h"
#include "ImageBuffer.h"



struct bad_source : public std::exception {
    const char* what() const noexcept override {
        return "Couldn't open the file.";
    }
};


struct bad_filesize : public std::exception {
    const char* what() const noexcept override {
        return "File is corrupted.";
    }
};



class ImageFormat {
public:
    virtual unsigned int load(Filepath &filepath, ImageBuffer* buffer) = 0;
    virtual unsigned int save(Filepath &filepath, ImageBuffer* buffer) = 0;
};

