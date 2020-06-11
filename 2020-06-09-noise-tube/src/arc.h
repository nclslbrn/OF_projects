#pragma once
#include "ofMain.h"

class Arc {
   public:
    Arc();
    deque<float> getArcAngles();
    float getInitialArcAngle();
    float initialAngle;
    deque<float> angles;
};