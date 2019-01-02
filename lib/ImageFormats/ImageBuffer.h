#pragma once
#include <vector>
#include "../DataStructures/Color.h"
#include "../DataStructures/Point.h"



class ImageBuffer {
public:
    ImageBuffer(int width=0, int height=0, int depth=8);
    void init(int width, int height, int depth=-1);
    void useDedicatedPalette(int nColors);
    void updateIndexMatrix();
    void updateBuffer();
    void dither();

    void px(int x, int y, Color color);
    void index(int x, int y, int index);
    void width(int width);
    void height(int height);
    void palette(std::vector<Color> palette);
    void palette(Color color);
    void grayscale(bool grayscale);

    Color px(int x, int y);
    std::vector<Color> palette();
    Color palette(int index);
    int paletteSize();
    int index(int x, int y);
    int width();
    int height();


private:
    std::vector<std::vector<Color>> _buffer;
    std::vector<std::vector<int>> _indexMatrix;

    int _width;
    int _height;
    int _depth;
    bool _grayscale;

    // used by dedicated palette generator
    struct Node{
        Node* parent;
        std::vector<Point<int>> pixels;
        std::vector<Node> children;
        bool chosen;
        int level;
        Color color;

        Node(Node* parentToSet=nullptr, int levelToSet=0) {
            parent = parentToSet;
            level = levelToSet;
            chosen = false;
        }
    };
    Node* _root;
    Color _quantify(Node* node);
    void _selectNodes(int amount, std::vector<Node*> &selected, int index=0);
    std::vector<Color> _palette;
};
