#pragma once
#include "ofMain.h"


class Point {
    public:
    Point();
    Point(ofVec2f _pos, ofColor _color, float _size);
    void draw();

    ofVec2f pos;
    ofColor color;
    float size;
};