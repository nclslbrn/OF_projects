#pragma once
#include "Layer.h"
#include "ofMain.h"
#include "ofxGifEncoder.h"

class ofApp : public ofBaseApp {
   public:
    void setup();
    void update();
    void draw();

    void keyPressed(int key);
    void onGifSaved(string& fileName);
    void exit();

   private:
    ofCamera cam;

    int numLayer = 8;
    int numFrame = 60;
    int cols = 36;
    int rows = 36;
    int cellSize, hSize;
    float depthStep = 0.05f;
    float noiseDepth = 0.0f;
    float noiseThreshold = 0.35;

    ofxGifEncoder gifEncoder;
    ofFbo cache;
    ofImage img;
    int currFrame = 0;
    bool willRecord, isRecording, isSaved, isOptimizing, isExported = false;
    vector<Layer> layers;
    vector<ofVec4f> lines;
};
