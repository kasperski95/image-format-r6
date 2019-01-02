#pragma once
#include "../Filepath.h"
#include "../DataStructures/Color.h"
#include "ImageBuffer.h"



struct bad_sourceFile : public std::exception {
    const char* what() const noexcept override {
        return "Couldn't open the file.";
    }
};


class ImageFormat {
public:
    virtual void load(Filepath &filepath, ImageBuffer* buffer) = 0;
    virtual void save(Filepath &filepath, ImageBuffer* buffer) = 0;
};

