#pragma once
#include "ofMain.h"

class Arc {
   public:
    Arc();
    void setNoiseRadius(float noiseRadius);
    void setNoiseScale(float noiseScale);
    void drawFromXandYRot(
        ofVec3f point,
        float xRot,
        float yRot,
        float radius,
        float t);
    deque<float> getArcAngles();
    float getInitialArcAngle();
    float initialAngle;
    float noiseRadius;
    float noiseScale;
    deque<float> angles;
};