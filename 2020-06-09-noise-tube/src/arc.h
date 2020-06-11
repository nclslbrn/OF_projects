#pragma once
#include "ofMain.h"

class Arc {
   public:
    Arc();
    void drawFromXandYRot(
        ofVec3f point,
        float xRot,
        float yRot,
        float radius);
    deque<float> getArcAngles();
    float getInitialArcAngle();
    float initialAngle;
    deque<float> angles;
};