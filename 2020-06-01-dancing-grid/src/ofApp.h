#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp {
   public:
    void setup();
    void update();
    void draw();
    void mouseMoved(int x, int y);

    vector<ofVec2f> initPoints, points;
    int columns = 8, rows = 8;
    float margin = 0.25;
    float noiseScale = 64;
    float radius = 8;
    int numFrame = 200;
    int initCellWidth, initCellHeight, xMargin, yMargin;
    // grid
    // elements doesn't have the size
    // element are linked
    // elements can collapse
};
