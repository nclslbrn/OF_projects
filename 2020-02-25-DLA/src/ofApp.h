#pragma once

#include "Walker.h"
#include "ofMain.h"

class ofApp : public ofBaseApp {
   public:
    void setup();
    void update();
    void draw();
    ofVec3f randomPos();

    deque<Walker> tree;
    deque<Walker> walkers;

    int walkerNum;
    float initSize;
    float decrease;

    ofEasyCam cam;
};
