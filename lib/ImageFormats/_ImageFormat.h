#pragma once
#include "../Filepath.h"
#include "../DataStructures/Color.h"
#include "ImageBuffer.h"

class ImageFormat {
public:
    virtual void load(Filepath const &filepath, ImageBuffer* buffer) = 0;
    virtual void save(Filepath const &filepath, ImageBuffer* buffer) = 0;
};
