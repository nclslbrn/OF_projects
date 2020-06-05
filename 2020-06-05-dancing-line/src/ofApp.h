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

    int cols = 256, rows = 256;
    int noiseScale = 130, animFrame = 60, currFrame = 0;
    double cellWidth, cellHeight;
    vector<double> noise;

    ofImage img;
    ofxGifEncoder gifEncoder;
    bool isRecording, isSaved, isOptimizing, isExported = false;
};
