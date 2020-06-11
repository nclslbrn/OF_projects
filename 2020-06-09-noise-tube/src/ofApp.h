#pragma once
#include "arc.h"
#include "ofMain.h"
#include "ofxCameraSaveLoad.h"
#include "ofxGifEncoder.h"
#include "particle.h"

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
    float radius = 100.0f;
    int numFrame{80};
    int currFrame = 0;
    ofVec2f particleSize = ofVec2f(2.0f, 15.0f);

    float extRadius, extRes;
    deque<Arc> arcs;
    deque<deque<Particle>> particles;
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
