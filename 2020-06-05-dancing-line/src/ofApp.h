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

    // must be even number
    int cols{3}, rows{3};
    int midWidthGrid, midHeightGrid;
    int noiseScale = 2, animFrame = 240, liveFrame = 30, currFrame = 0;
    double cellWidth, cellHeight, cellDiag;
    float noiseRadius = 2;
    vector<vector<ofVec2f>> linePoints;
    ofImage img;
    ofxGifEncoder gifEncoder;
    bool willRecord, isRecording, isSaved, isOptimizing, isExported = false;
};
