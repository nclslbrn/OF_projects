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

    void drawParticle();
    void drawFromXandYRot(
        ofVec3f point,
        float xRot,
        float yRot,
        float radius,
        float t);
    void updateParticle();
    void init();

    ofVec3f getParticlePos();
    ofVec2f getParticleSize();

    ofVec3f initPos;
    ofVec3f initRot;
    ofVec3f endRot;
    ofPlanePrimitive plane;
};