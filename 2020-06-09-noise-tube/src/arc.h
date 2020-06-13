#pragma once
#include "ofMain.h"

class Arc {
   public:
    Arc(float depth);
    void setNoiseRadius(float noiseRadius);
    void setNoiseScale(float noiseScale);

    void drawFromXandYRot(
        ofVec3f point,
        float xRot,
        float yRot,
        float radius,
        float t,
        ofVec3f nextpoint);

    deque<float> getArcAngles();
    float getInitialArcAngle();
    ofVec3f getNoisedVector(ofVec3f orig, float t);

    float initialAngle;
    float noiseRadius;
    float noiseScale;
    float depth;
    deque<float> angles;
};