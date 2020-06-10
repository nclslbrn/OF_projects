#pragma once

#include "ofMain.h"
#include "ofxCameraSaveLoad.h"
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

    ofImage img;
    ofxGifEncoder gifEncoder;
    ofEasyCam cam;
    vector<ofVec3f> curve;
    vector<vector<float>> ellipseSectionAngles;
    float arcRadius;
    float radius;
    int numFrame = 60, currFrame = 60;
    float res = 0.35f;
    float cubeSize;
    int outerSteps, innerSteps;
    ofBoxPrimitive box;
    ofLight light;

    bool willRecord, isRecording, isSaved, isOptimizing, isExported = false;
    string camPresetFile = "camera.preset";
};
