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

    deque<Arc> arcs;
    float res = 0.02f;
    float radius = 140.0f;
    int numFrame{30};
    int currFrame = 0;
    float extRadius, extRes, innerStepDistance;
    int outerSteps, innerSteps;
    float noiseScale;
    float noiseRadius;

    bool isDisplaced = true;

    ofLight light;
    ofImage img;
    ofEasyCam cam;
    ofxGifEncoder gifEncoder;
    string camPresetFile = "camera.preset";
    bool willRecord, isRecording, isSaved, isOptimizing, isExported = false;
};
