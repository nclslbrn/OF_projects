#pragma once
#include "ofMain.h"

class Voxel {
   public:
    Voxel();
    Voxel(ofVec3f _pos, ofColor _col, float _size);

    void update();
    void draw();

    ofVec3f pos;
    ofColor col;
    float size;
};