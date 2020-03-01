#include "Point.h"

Point::Point() {
    pos = ofVec2f(0, 0);
    color = ofColor::black;
    size = 1.0;
}

Point::Point(ofVec2f _pos, ofColor _color, float _size) {
    pos = _pos;
    color = _color;
    size = _size;
}

void Point::draw() {
    ofSetColor(color);
    ofDrawCircle(pos.x, pos.y, size);
}