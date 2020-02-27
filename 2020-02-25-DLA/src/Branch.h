#pragma once
#include "ofMain.h"

class Branch {
   public:
    Branch();
    Branch(ofVec3f _start, ofVec3f _end, float _weight);
    void draw();
    
    ofVec3f start, end;
    float weight;
};