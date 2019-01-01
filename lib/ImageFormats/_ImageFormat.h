#pragma once
#include "../Filepath.h"
#include "../DataStructures/Color.h"
#include "ImageBuffer.h"

class ImageFormat {
public:
    virtual void load(Filepath &filepath, ImageBuffer* buffer) = 0;
    virtual void save(Filepath &filepath, ImageBuffer* buffer) = 0;
};

