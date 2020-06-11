#pragma once
#include "arc.h"
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
    void mouseDragged(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void onGifSaved(string& fileName);
    void exit();
    float ease(float t);

    float res = 0.1f;
    int numFrame{80};
    int currFrame = 0;
    float extRadius, extRes;
    float radius;
    deque<Arc> arcs;
    int arcsSize;
    float cubeSize;
    int outerSteps, innerSteps;
    float noiseScale;
    float noiseRadius;
    ofBoxPrimitive box;
    ofLight light;
    ofImage img;
    ofEasyCam cam;
    ofxGifEncoder gifEncoder;
    string camPresetFile = "camera.preset";
    bool isDisplaced = true;
    bool willRecord, isRecording, isSaved, isOptimizing, isExported = false;
};
