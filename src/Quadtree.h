//
// Created by zackm on 2025-10-14.
//

#ifndef OF_CLION_MINIMAL_QUADTREE_H
#define OF_CLION_MINIMAL_QUADTREE_H
#include <ofRectangle.h>

#include "ShapeDefs.h"


class Quadtree {
public:
    ofRectangle bounds;
    int capacity;
    std::vector<Shape> shapes;
    bool divided = false;

    std::unique_ptr<Quadtree> northeast, northwest, southeast, southwest;

    Quadtree(const ofRectangle& _bounds, int _capacity = 4)
        : bounds(_bounds), capacity(_capacity) {}

    void subdivide();
    bool insert(const Shape& s);
    void query(const ofRectangle& range, std::vector<Shape>& found);
    void draw();
};


#endif //OF_CLION_MINIMAL_QUADTREE_H