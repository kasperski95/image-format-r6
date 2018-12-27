#pragma once
#include <vector>
#include "../DataStructures/Color.h"
#include "../DataStructures/Point.h"

using colorType = unsigned int;

class ImageBuffer {
public:
    ImageBuffer(int width=0, int height=0, int depth=8);
    void init(int width, int height, int depth=-1);
    void useDedicatedPalette(int bytes);

    Color<colorType> px(int x, int y);
    void px(int x, int y, Color<colorType> color);
    void width(int width);
    void height(int height);
    int width();
    int height();
private:

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
    void _chooseNodes(int index, int amount, std::vector<Node*> &chosen);

    std::vector<std::vector<Color<colorType>>> _buffer;
    int _width;
    int _height;
    int _depth;


};
