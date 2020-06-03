#pragma once

#include "ofMain.h"
#include "ofxGifEncoder.h"

class ofApp : public ofBaseApp {
   public:
    void setup();
    void update();
    void draw();
    void mouseMoved(int x, int y);
    void onGifSaved(string& fileName);
    void keyPressed(int key);
    void exit();

    float ease(float p);
    bool isRecording, isSaved, isOptimizing, isExported = false;
    vector<ofVec2f> initPoints, points;
    int columns{4}, rows{4}, currFrame{0}, animFrame{96};
    float marginFactor{0.25}, noiseScale{64}, radius{8};
    int initCellWidth, initCellHeight;
    ofVec2f margin;

    ofImage img;
    ofxGifEncoder gifEncoder;
};
