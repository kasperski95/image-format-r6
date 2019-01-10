#include <iostream>     // for logging
#include <algorithm>    // sort
#include <cassert>
#include <math.h>
#include <utility>      // pair
#include "ImageBuffer.h"



ImageBuffer::ImageBuffer(int widthToSet, int heightToSet, int depthToSet) {
    this->init(widthToSet, heightToSet, depthToSet);
    _root = nullptr;
    _grayscale = false;
}

ImageBuffer::~ImageBuffer() {
    if (_root) delete _root;
}


void ImageBuffer::init(int widthToSet, int heightToSet, int depthToSet) {
    _width = widthToSet;
    _height = heightToSet;

    if (depthToSet != -1)
        _depth = depthToSet;

    _buffer.resize(_width);
    for(int i = 0; i < _buffer.size(); ++i)
        _buffer[i].resize(_height);

    _indexMatrix.resize(_width);
    for(int x = 0; x < _indexMatrix.size(); ++x) {
        for(int y = 0; y < _height; ++y) {
            _indexMatrix[x].push_back(-1);
        }
    }
}


// GENERATORS
void ImageBuffer::generatePalette(int nColors) {
    // generate octree
    _root = new Node();
    for (int y = 0; y < _height; ++y) {
        for (int x = 0; x < _width; ++x) {
            _root->pixels.push_back(Point<int>(x,y));
        }
    }
    _quantify(_root);

    // choose best colors
    std::vector<Node*> selectedNodes;
    selectedNodes.push_back(_root);
    _selectNodes(nColors, selectedNodes);

    // fill palette
    for (int i = 0; i < selectedNodes.size(); ++i) {
       _palette.push_back(selectedNodes[i]->color);
    }

    // sort palette and update indexes
    this->generateMatrix();
    _sortPaletteByColorPopularity();
    this->generateMatrix();
}


void ImageBuffer::generateBuffer() {
    for (int y = 0; y < _height; ++y) {
        for (int x = 0; x < _width; ++x) {
            _buffer[x][y] = _palette[_indexMatrix[x][y]];
        }
    }
}


void ImageBuffer::generateMatrix() {
    int bestIndex, minColorDistance, distance;
    for (int y = 0; y < _height; ++y) {
        for (int x = 0; x < _width; ++x) {
            for (int i = 0; i < _palette.size(); ++i) {
                if (!_grayscale)
                    distance = abs(_buffer[x][y].r - _palette[i].r) + abs(_buffer[x][y].g - _palette[i].g) + abs(_buffer[x][y].b - _palette[i].b);
                else
                    distance = abs(_buffer[x][y].r - _palette[i].r) * 0.299 + abs(_buffer[x][y].g - _palette[i].g) * 0.587 + abs(_buffer[x][y].b - _palette[i].b) * 0.114;

                if (i == 0) {
                    bestIndex = 0;
                    minColorDistance = distance;
                    continue;
                }

                if (distance < minColorDistance) {
                    minColorDistance = distance;
                    bestIndex = i;
                }
            }
            _indexMatrix[x][y] = bestIndex;
        }
    }
}


void ImageBuffer::generateBufferFromPalette(int nSquaresInRow) {
    this->init(nSquaresInRow, _palette.size() / nSquaresInRow);

    for (int i = 0; i < _palette.size(); ++i) {
        _buffer[i/nSquaresInRow][i % nSquaresInRow] = _palette[i];
    }
}


// DITHERING
void ImageBuffer::dither() {
    std::vector<std::vector<Color>> errors;
    errors.resize(_width + 1);
    for(int i = 0; i < errors.size(); ++i)
        errors[i].resize(_height + 1);

    Color error, minError;
    int colorID;

    for (int y = 0; y < _height; ++y) {
        for (int x = 0; x < _width; ++x) {

            for (int i = 0; i < _palette.size(); ++i) {
                error.r = _buffer[x][y].r + errors[x][y].r - _palette[i].r;
                error.g = _buffer[x][y].g + errors[x][y].g - _palette[i].g;
                error.b = _buffer[x][y].b + errors[x][y].b - _palette[i].b;
                if (i == 0) {
                    colorID = 0;
                    minError = error;
                    continue;
                }

                if (error < minError) {
                    minError = error;
                    colorID = i;
                }
            }

            _buffer[x][y] = _palette[colorID];
            errors[x][y+1] += minError * (7.f/16);
            if (y > 0)
                errors[x+1][y-1] += minError * (3.f/16);
            errors[x+1][y] += minError * (5.f/16);
            errors[x+1][y+1] += minError * (1.f/16);
        }
    }

    this->generateMatrix();
}


// DEDICATED PALETTE FROM OCTREE
Color ImageBuffer::_quantify(Node* node) {
    if (node) {
        if (node->pixels.size() == 0)
            return Color();

        // base case
        for (int i = 0; i < node->pixels.size(); ++i) {
            if (_buffer[node->pixels[0].x][node->pixels[0].y] != _buffer[node->pixels[i].x][node->pixels[i].y]) {break;}

            if (i == node->pixels.size() - 1) {
                node->color = _buffer[node->pixels[0].x][node->pixels[0].y];
                return node->color;
            }
        }

        // add 8 children (octree)
        for (int i = 0; i < 8; ++i) {
            node->children.push_back(Node(node, node->level + 1));
        }

        // split pixels across children
        int childIndex;
        int x, y;
        for (int i = 0; i < node->pixels.size(); ++i) {
            x = node->pixels[i].x;
            y = node->pixels[i].y;

            childIndex  = (_buffer[x][y].r >> (_depth - node->level - 1)) % 2;
            childIndex += ((_buffer[x][y].g >> (_depth - node->level - 1)) % 2) * 2;
            childIndex += ((_buffer[x][y].b >> (_depth - node->level - 1)) % 2) * 4;
            node->children[childIndex].pixels.push_back(Point<int>(x,y));
        }

        // get avg color
        for (int i = 0; i < 8; ++i) {
            node->color += _quantify(&(node->children[i])) * ((float)(node->children[i].pixels.size()) / node->pixels.size());
        }

        return node->color;
    }
}


void ImageBuffer::_selectNodes(int limit, std::vector<Node*> &selected, int index) {
    if (selected[index]->children.size() > 0) {
        bool addedAllChildren = true;
        int i;
        int pixelsToSubtract = 0;

        // keep adding children to the selected list
        for (i = 0; i < selected[index]->children.size(); ++i) {
            if (selected.size() < limit || (selected.size() < limit && i == 7)) {
                // add children to the list
                if (selected[index]->children[i].pixels.size() > 0) {
                    selected.push_back(&(selected[index]->children[i]));
                    pixelsToSubtract += selected[index]->children[i].pixels.size();
                }
            } else {
                addedAllChildren = false;
                break;
            }
        }

        if (addedAllChildren) {
            // remove node from the selected list, because all children are included
            std::swap(selected[index], selected[selected.size() - 1]);
            selected.pop_back();
        } else {
            // update node's color based on not included children
            selected[index]->color = Color();
            for (; i < selected[index]->children.size(); ++i) {
                selected[index]->color += selected[index]->children[i].color * ((float)selected[index]->children[i].pixels.size() / (selected[index]->pixels.size() - pixelsToSubtract));
            }
        }
    }

    // recurrence
    if (index + 1 < selected.size()) {
        _selectNodes(limit, selected, index + 1);
    }
}


void ImageBuffer::_sortPaletteByColorPopularity() {
    using colorOccurance = std::pair<int,int>;
    std::vector<colorOccurance> occurances(_palette.size());
    for (int i = 0; i < _palette.size(); ++i) {
        occurances[i].first = i;
    }
    for (int y = 0; y < _height; ++y) {
        for (int x = 0; x < _width; ++x) {
            occurances[_indexMatrix[x][y]].second++;
        }
    }
    std::sort(occurances.begin(), occurances.end(), [](colorOccurance &l, colorOccurance r) {
        return l.second > r.second;
    });

    for (int i = 0; i < _palette.size(); ++i) {
        std::swap(_palette[i], _palette[occurances[i].first]);
    }
}


// GETTERS
std::vector<Color> ImageBuffer::palette() {return _palette;}
Color ImageBuffer::px(int x, int y) {return _buffer[x][y];}
Color ImageBuffer::palette(int index) {return _palette[index];}
int ImageBuffer::paletteSize() {return _palette.size();}
int ImageBuffer::width() {return _width;}
int ImageBuffer::height() {return _height;}
int ImageBuffer::index(int x, int y) {return _indexMatrix[x][y];}

// SETTERS
void ImageBuffer::palette(std::vector<Color> paletteToSet) {_palette = paletteToSet; this->generateMatrix(); _sortPaletteByColorPopularity(); this->generateMatrix();}
void ImageBuffer::palette(Color color) {_palette.push_back(color);}
void ImageBuffer::px(int x, int y, Color color) {_buffer[x][y] = color;}
void ImageBuffer::width(int widthToSet) {_width = widthToSet;}
void ImageBuffer::height(int heightToSet) {_height = heightToSet;}
void ImageBuffer::index(int x, int y, int newIndex) {_indexMatrix[x][y] = newIndex;}
void ImageBuffer::grayscale(bool value) {_grayscale = value;}


