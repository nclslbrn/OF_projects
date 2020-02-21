#include "Voxel.h"

Voxel::Voxel() {
    pos.set(ofGetWidth() / 2.0, ofGetHeight() / 2.0, 0);
    col.set(255, 255, 255);
}

Voxel::Voxel(ofVec3f _pos, ofColor _col, float _size) {
    pos.set(_pos.x, _pos.y, _pos.z);
    col = _col;
    size = _size;
}

void Voxel::draw() {
    ofSetColor(col);
    ofFill();
    ofDrawBox(pos.x, pos.y, pos.z, size, size, size);
}

void Voxel::update() {
}