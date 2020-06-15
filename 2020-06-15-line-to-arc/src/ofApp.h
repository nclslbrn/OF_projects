#pragma once

#include "ofMain.h"
#include "ofxGifEncoder.h"

class ofApp : public ofBaseApp {
   public:
    void setup();
    void update();
    void draw();

    float ease(float p);
    void keyPressed(int key);
    void onGifSaved(string& fileName);
    void animation(float t);
    void exit();

    vector<ofVec2f> positions;
    vector<float> distances;
    vector<float> thetas;

    float angle;
    float initRadius;
    int numArcs = 1280;
    float circleRes = 0.15;
    float margin = 64;
    bool isDebugActive = false;

    // Motion blur
    int samplesPerFrame = 30;
    float shutterAngle = 2.0f;
    // Gif animation / export setup
    ofxGifEncoder gifEncoder;
    int numFrames{30};
    int currFrame = 0;
    ofImage img;
    bool willRecord, isRecording, isSaved, isOptimizing, isExported = false;
};
