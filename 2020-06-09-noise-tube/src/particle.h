#pragma once
#include "ofMain.h"

class Particle {
   public:
    Particle();
    Particle(
        ofVec3f initPos,
        float width,
        float height,
        float radius);
    void draw();
    void update();
    void init();
    ofVec3f initPos;
    ofVec3f initRot;
    ofPlanePrimitive plane;
};