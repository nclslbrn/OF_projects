#pragma once
#include "Walker.h"
#include "ofMain.h"
#include "ofxCsv.h"
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
    void onButtonEvent(ofxDatGuiButtonEvent e);
    void exportTree();

    ofVec3f randomPos();
    ofVec3f onEmitter();
    ofColor indexColor(int index);
    string getInfoString();

    deque<Walker> tree;
    deque<Walker> walkers;
    ofLight wLight, oLight, bLight;
    ofMaterial treeMat;
    ofCamera cam;

    ofxDatGui* gui;
    ofParameter<int> walkerNum, steps, treeSize;
    ofParameter<float> initWalkerSize;
    ofParameter<float> initEmitterDistance;
    ofParameter<int> decreaseParam;
    ofParameter<int> increaseParam;

    double decreaseWalkerSize, increaseEmitterDistance;
    string factorInfo;

    ofxCsv csv;
    ofxCsv csvRecorder;

    float walkerSize, xRot, zRot, emitterDistance;
    int zoom, mouseScrollSensivity;
    bool isExporting;
};
