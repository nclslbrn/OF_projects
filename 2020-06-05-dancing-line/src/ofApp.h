#pragma once

#include "ofMain.h"
#include "ofxGifEncoder.h"

class ofApp : public ofBaseApp {
   public:
    void setup();
    void update();
    void draw();

    void keyPressed(int key);
    void mouseMoved(int x, int y);
    void onGifSaved(string& fileName);
    void exit();
    float ease(float p);
    float ease(float p, float g);
    int cols{3}, rows{3};
    int midWidthGrid, midHeightGrid;
    int noiseScale = 2, animFrame = 120, currFrame = 0;
    double cellWidth, cellHeight, cellDiag;
    float noiseRadius = 2;
    vector<vector<ofVec2f>> linePoints;
    vector<ofColor> lineColor;
    ofImage img;
    ofxGifEncoder gifEncoder;
    bool willRecord, isRecording, isSaved, isOptimizing, isExported = false;
};
