//
// Created by zackm on 2025-10-14.
//

#include "Quadtree.h"

void Quadtree::subdivide() {
    float x = bounds.x;
    float y = bounds.y;
    float w = bounds.width / 2;
    float h = bounds.height / 2;

    northeast = std::make_unique<Quadtree>(ofRectangle(x + w, y, w, h), capacity);
    northwest = std::make_unique<Quadtree>(ofRectangle(x, y, w, h), capacity);
    southeast = std::make_unique<Quadtree>(ofRectangle(x + w, y + h, w, h), capacity);
    southwest = std::make_unique<Quadtree>(ofRectangle(x, y + h, w, h), capacity);

    divided = true;
}

bool Quadtree::insert(const Shape& s) {
    ofRectangle shapeBounds;
    shapeBounds.set(
        std::min(s.start.x, s.end.x),
        std::min(s.start.y, s.end.y),
        std::abs(s.end.x - s.start.x),
        std::abs(s.end.y - s.start.y)
    );

    if (!bounds.intersects(shapeBounds)) return false;

    if (shapes.size() < capacity && !divided) {
        shapes.push_back(s);
        return true;
    }

    if (!divided) subdivide();

    if (northeast->insert(s)) return true;
    if (northwest->insert(s)) return true;
    if (southeast->insert(s)) return true;
    if (southwest->insert(s)) return true;

    return false;
}

void Quadtree::query(const ofRectangle& range, std::vector<Shape>& found) {
    if (!bounds.intersects(range)) return;

    for (auto& s : shapes) {
        ofRectangle shapeBounds(
            std::min(s.start.x, s.end.x),
            std::min(s.start.y, s.end.y),
            std::abs(s.end.x - s.start.x),
            std::abs(s.end.y - s.start.y)
        );
        if (range.intersects(shapeBounds)) {
            found.push_back(s);
        }
    }

    if (divided) {
        northeast->query(range, found);
        northwest->query(range, found);
        southeast->query(range, found);
        southwest->query(range, found);
    }
}

void Quadtree::draw() {
    ofNoFill();
    ofSetColor(0, 255, 0, 100);
    ofDrawRectangle(bounds);
    if (divided) {
        northeast->draw();
        northwest->draw();
        southeast->draw();
        southwest->draw();
    }
}