#pragma once

#include "ofMain.h"
#include "ofxGifEncoder.h"

class ofApp : public ofBaseApp {
   public:
    void setup();
    void update();
    void draw();

    void keyPressed(int key);
    void onGifSaved(string& fileName);
    void animation(float t);
    void exit();

    // Motion blur
    ofFbo cache;
    int samplesPerFrame = 20;
    float shutterAngle = 4.0f;

    // Gif animation / export setup
    ofxGifEncoder gifEncoder;
    int numFrames{40};
    int currFrame = 0;
    ofImage img;
    bool willRecord, isRecording, isSaved, isOptimizing, isExported = false;

    // Animation variables
    ofVec2f p1, v1, center;
    int n = 24;
    int radius = 140;
    int arcs = 6;
    int arcLength = 26;
};
