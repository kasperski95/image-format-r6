#include "ImageBuffer.h"
#include <algorithm>
#include <iostream> // just for production

ImageBuffer::ImageBuffer(int widthToSet, int heightToSet, int depthToSet) {
    this->init(widthToSet, heightToSet, depthToSet);
    _root = nullptr;
}

void ImageBuffer::init(int widthToSet, int heightToSet, int depthToSet) {
    _width = widthToSet;
    _height = heightToSet;

    if (depthToSet != -1)
        _depth = depthToSet;

    _buffer.resize(_width);

    for(int i = 0; i < _buffer.size(); ++i) {
        _buffer[i].resize(_height);
    }
}

void ImageBuffer::useDedicatedPalette(int colors) {
    _root = new Node();
    for (int y = 0; y < _height; ++y) {
        for (int x = 0; x < _width; ++x) {
            _root->pixels.push_back(Point<int>(x,y));
        }
    }

    _quantify(_root);

    std::vector<Node*> chosenNodes;
    chosenNodes.push_back(_root);
    _chooseNodes(0, colors, chosenNodes);

    for (int i = 0; i < chosenNodes.size(); ++i) {
       chosenNodes[i]->color.print();
    }

}

void ImageBuffer::_chooseNodes(int index, int amt, std::vector<Node*> &chosen) {
    if (chosen[index]->children.size() > 0) {
        bool addedAllChildren = true;
        int i;
        int pixelsToSubtract = 0;

        // keep adding children to the chosen list
        for (i = 0; i < chosen[index]->children.size(); ++i) {
            if (chosen.size() < amt) {
                // add children to the list
                if (chosen[index]->children[i].pixels.size() > 0) {
                    chosen.push_back(&(chosen[index]->children[i]));
                    pixelsToSubtract += chosen[index]->children[i].pixels.size();
                }
            } else {
                // prevent overflow
                addedAllChildren = false;
                break;
            }
        }


        if (addedAllChildren) {
            // remove node from the chosen list, because all children are included
            std::swap(chosen[index], chosen[chosen.size() - 1]);
            chosen.pop_back();
        } else {
            // new color for node
            chosen[index]->color = Color<colorType>();
            for (; i < chosen[index]->children.size(); ++i) {
                chosen[index]->color += chosen[index]->children[i].color * ((float)chosen[index]->children[i].pixels.size() / (chosen[index]->pixels.size() - pixelsToSubtract));
            }
        }
    }

    // recurence
    if (index + 1 < chosen.size()) {
        _chooseNodes(index + 1, amt, chosen);
    }
}

Color<colorType> ImageBuffer::_quantify(Node* node) {
    if (node) {
        if (node->pixels.size() == 0)
            return Color<colorType>();

        // base case
        for (int i = 0; i < node->pixels.size(); ++i) {
            if (_buffer[node->pixels[0].x][node->pixels[0].y] != _buffer[node->pixels[i].x][node->pixels[i].y]) {break;}

            if (i == node->pixels.size() - 1) {
                node->color = _buffer[node->pixels[0].x][node->pixels[0].y];
                return node->color;
            }
        }


        // add 8 children
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

void ImageBuffer::px(int x, int y, Color<colorType> color) {_buffer[x][y] = color;}
void ImageBuffer::width(int widthToSet) {_width = widthToSet;}
void ImageBuffer::height(int heightToSet) {_height = heightToSet;}

Color<colorType> ImageBuffer::px(int x, int y) {return _buffer[x][y];}
int ImageBuffer::width() {return _width;}
int ImageBuffer::height() {return _height;}

