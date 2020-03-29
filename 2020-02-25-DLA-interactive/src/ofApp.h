#pragma once
#include "Walker.h"
#include "ofMain.h"
#include "ofxDatGui.h"

class ofApp : public ofBaseApp {
   public:
    void setup();
    void init();
    void update();
    void draw();
    void initGui();
    void initDatGui();

    void mouseMoved(int x, int y);
    void mouseScrolled(int x, int y, float scrollX, float scrollY);
    void onSliderEvent(ofxDatGuiSliderEvent e);

    ofVec3f randomPos();
    ofVec3f onEmitter();
    ofColor indexColor(int index);

    deque<Walker> tree;
    deque<Walker> walkers;
    ofLight pointLight;
    ofMaterial treeMat;
    ofCamera cam;

    ofxDatGui* gui;
    ofParameter<int> walkerNum, steps, treeSize;
    ofParameter<float> initWalkerSize;
    ofParameter<float> decreaseWalkerSize;
    ofParameter<float> initEmitterDistance;
    ofParameter<float> increaseEmitterDistance;

    float walkerSize, xRot, zRot, emitterDistance;
    int zoom, mouseScrollSensivity;
};
