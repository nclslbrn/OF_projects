#pragma once
#include "ofMain.h"

class Walker {
   public:
    Walker();
    Walker(ofVec3f _pos, float _size);

    void walk();
    int isCloseTo(deque<Walker> walkers);

    ofVec3f pos;
    float size;
    bool isStuck;
};