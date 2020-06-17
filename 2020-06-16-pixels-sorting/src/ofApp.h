#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp {
   public:
    void setup();
    void update();
    void draw();
    void keyPressed(int key);
    void addLine(int x, int fromY, int toY, ofColor pixColor, int brightness, bool goDown);
    ofImage source;
    int pix = 0;
    int currY;
    int lineLenght = 64;
    int width, height;
    int numFrames{30};
    int currFrame = 0;
    int threshold = 1;
    float t;
};
