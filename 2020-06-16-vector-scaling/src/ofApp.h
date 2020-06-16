#pragma once

#include "ofMain.h"
#include "ofxGifEncoder.h"

class ofApp : public ofBaseApp {
   public:
    void setup();
    void update();
    void draw();
    void keyPressed(int key);
    void mousePressed(int x, int y, int button);
    void mouseMoved(int x, int y);
    void onGifSaved(string& fileName);
    void animation(float t);
    void exit();
    float ease(float p);

    vector<ofVec2f> pos;
    vector<string> strings;
    int size{512};

    ofVec2f split;
    ofTrueTypeFont font;

    // Motion blur
    ofFbo cache;
    int samplesPerFrame = 1;
    float shutterAngle = 4.0f;

    // Gif animation / export setup
    ofxGifEncoder gifEncoder;
    int numFrames{30};
    int currFrame = 0;
    ofImage img;
    bool willRecord, isRecording, isSaved, isOptimizing, isExported = false;
};
