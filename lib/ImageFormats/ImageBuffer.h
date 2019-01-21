#pragma once
#include <vector>
#include "../DataStructures/Color.h"
#include "../DataStructures/Point.h"



class ImageBuffer {
public:
    ImageBuffer(int width=0, int height=0, int channelDepth=8);
    ~ImageBuffer();
    void init(int width, int height, int channelDepth=-1);

    // GENERATORS
    void generatePalette(int nColors);  // from buffer
    void generateMatrix();              // from buffer & matrix
    void generateBuffer();              // from palette & matrix
    void generateBufferFromPalette(int nSquaresInRow=8);

    // DITHERING
    void dither();

    // GETTERS
    std::vector<Color> palette();
    Color px(int x, int y);
    Color palette(int index);
    int paletteSize();
    int index(int x, int y);
    int width();
    int height();
    int channelDepth();

    // SETTERS
    void px(int x, int y, Color color);
    void index(int x, int y, int index);
    void width(int width);
    void height(int height);
    void palette(std::vector<Color> palette);
    void palette(Color color);
    void grayscale(bool grayscale);


private:
    std::vector<std::vector<Color>> _buffer;
    std::vector<std::vector<int>> _indexMatrix;
    std::vector<Color> _palette;
    int _width;
    int _height;
    int _channelDepth;
    bool _grayscale;

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

    //-----------------------------------------------------------------

    // DEDICATED PALETTE STUFF
    Color _quantify(Node* node);
    void _selectNodes(int amount, std::vector<Node*> &selected, int index=0);

    void _sortPaletteByColorPopularity();
};
