#pragma once
#include "Walker.h"
#include "ofMain.h"
#include "ofxGui.h"

class ofApp : public ofBaseApp {
   public:
    void setup();
    void init();
    void update();
    void draw();
    void initGui();

    void walkerNumChanged(int& walkerNum);
    void stepsValueChaged(int& steps);
    void initWalkersSizeChanged(float& initWalkerSize);
    void decreaseValueChanged(float& decrease);
    void radiusValueChanged(float& radius);
    void treeSizeChanged(int& treeSize);

    void mouseMoved(int x, int y);
    void mouseScrolled(int x, int y, float scrollX, float scrollY);

    ofVec3f randomPos();
    ofVec3f onRadius();
    ofColor indexColor(float indexPercent);

    deque<Walker> tree;
    deque<Walker> walkers;
    ofLight pointLight;

    ofxPanel gui;
    ofParameter<int> walkerNum, steps, treeSize;
    ofParameter<float> initWalkerSize, decrease, radius;
    float walkerSize, xRot, zRot;
    int zoom, mouseScrollSensivity;
};
