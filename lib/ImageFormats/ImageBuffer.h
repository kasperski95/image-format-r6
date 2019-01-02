#pragma once
#include <vector>
#include "../DataStructures/Color.h"
#include "../DataStructures/Point.h"

using colorType = unsigned int;

class ImageBuffer {
public:
    ImageBuffer(int width=0, int height=0, int depth=8);
    void init(int width, int height, int depth=-1);
    void useDedicatedPalette(int nColors);
    void updateIndexMatrix(std::vector<Color<unsigned int>> const &palette);
    void updateBuffer();
    void dither(std::vector<Color<colorType>> const &palette);

    void px(int x, int y, Color<colorType> color);
    void index(int x, int y, int index);
    void width(int width);
    void height(int height);
    void palette(Color<colorType> color);

    Color<colorType> px(int x, int y);
    std::vector<Color<colorType>> palette();
    Color<colorType> palette(int index);
    int paletteSize();
    int index(int x, int y);
    int width();
    int height();


private:
    std::vector<std::vector<Color<colorType>>> _buffer;

    std::vector<std::vector<int>> _indexMatrix;

    int _width;
    int _height;
    int _depth;

    // used by dedicated palette generator
    struct Node{
        Node* parent;
        std::vector<Point<int>> pixels;
        std::vector<Node> children;
        bool chosen;
        int level;
        Color<colorType> color;

        Node(Node* parentToSet=nullptr, int levelToSet=0) {
            parent = parentToSet;
            level = levelToSet;
            chosen = false;
        }
    };
    Node* _root;
    Color<colorType> _quantify(Node* node);
    void _selectNodes(int amount, std::vector<Node*> &selected, int index=0);
    std::vector<Color<colorType>> _palette;
};
